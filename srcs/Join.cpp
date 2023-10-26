
#include "Join.hpp"
#include "Part.hpp"
#include "ToolFunctions.hpp"
#include "defines.hpp"

void
Join::joinCommand(	const int		&socket,
					std::string		full_command,
					t_server_mode 	&_serverSettings)
{
	std::vector<std::string> command_parts;
	std::vector<std::string> temp_channels;
	std::vector<std::string> temp_keys;
	ToolFunctions::splitCommandInParts(full_command, command_parts);

	if (command_parts.size() == 2 && _handleSpecialCases(socket, command_parts, _serverSettings))
		return ;
	ToolFunctions::parseIntoParts(command_parts, 1, temp_channels);
	if (command_parts.size() == 3)
		ToolFunctions::parseIntoParts(command_parts, 2, temp_keys);
	for (std::vector<std::string>::size_type i = 0; i < temp_channels.size(); ++i) // Go through channels candidates
	{
		if (!ToolFunctions::doesChannelExistWithName(temp_channels.at(i), _serverSettings.channels)) // Channel does not exist so create a new one
		{
			_channelDoesNotExistHelper(socket, full_command, i, temp_channels, temp_keys, _serverSettings); continue ;
		}
		for (std::vector<Channel>::size_type k = 0; k < _serverSettings.channels.size(); ++k) // Find existing channels
		{
			if (temp_channels.at(i) == _serverSettings.channels.at(k).getChannelName())
			{
				if (_serverSettings.channels.at(k).isOnChannel(socket)) // Client is already on channel
				{	// On purpose do nothing if already on a channel, different servers react differently
					// Some do nothing, some part and rejoin user, there is no standard way to handle this
					break;
				} else if (_serverSettings.channels.at(k).howManyMembersOnChannel() >= _serverSettings.channels.at(k).getUserLimit())
				{
					_channelIsFullErrorHelper(socket, k, _serverSettings); break;
				} else if (_serverSettings.channels.at(k).isInviteOnly() && !_serverSettings.channels.at(k).isClientInvited(socket)) // Invite only channel
				{
					_inviteOnlyErrorHelper(socket, k, _serverSettings); break;
				} else if (_serverSettings.channels.at(k).isThereKey()) // Does Channel have key set (password)
				{
					if (i < temp_keys.size()) // Client provides key
					{
						if (_serverSettings.channels.at(k).doesKeyMatch(temp_keys.at(i))) // Key matches
						{
							_keyMatchesHelper(socket, full_command, k, _serverSettings); break ;
						}else // Key does not match
						{
							_keyDoesNotMatchErrorHelper(socket, k, _serverSettings); break ;
						}
					}else // Client does not provide key
					{
						_clientDoesNotProvideKeyErrorHelper(socket, k, _serverSettings); break ;
					}
				}else // Channel does not have a key
				{
					_channelDoesNotHaveKeyHelper(socket, k, full_command, _serverSettings); break ;
				}
			}
		}
	}
}

int
Join::_handleSpecialCases(	const int						&socket,
							const std::vector<std::string>	&command_parts,
							t_server_mode 					&_serverSettings)
{
	if (command_parts.at(1) == ":")	// For compatibility with irssi, ":" Is not in a Standard
		return (1);
	if (command_parts.at(1) == "0") // Part from all channels
	{
		Part::partFromAllChannels(socket, _serverSettings);
		return (1);
	}
	return (0);
}

void
Join::_channelDoesNotExistHelper(	const int 									&socket,
									const std::string							&full_command,
									const std::vector<std::string>::size_type	&i,
									std::vector<std::string>					&temp_channels,
									const std::vector<std::string>				&temp_keys,
									t_server_mode 								&_serverSettings)
{
	if (temp_channels.at(i).size() > CHANNELLEN)
		temp_channels.at(i) = temp_channels.at(i).substr(0, CHANNELLEN);
	try
	{
		_serverSettings.channels.push_back(Channel(temp_channels.at(i), socket)); // Create Channel
	} catch (const std::exception& e)
	{
		std::stringstream ss;
		ss << temp_channels.at(i);
		ss << " :Too many channels";
		Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients),ERR_TOOMANYCHANNELS, ss.str(), _serverSettings.debug);
		ss.str("");
		return ;
	}
	// Channel creation succesfull
	_serverSettings.channels.at(_serverSettings.channels.size() - 1).giveOps(socket); //Give ops to only user
	if (i < temp_keys.size()) // If there is a key candidate for channel then set up channel key
		_serverSettings.channels.at(_serverSettings.channels.size() - 1).setKey(temp_keys.at(i));

	std::stringstream ss;
	ss << ":"; // Join messsage
	ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
	ss <<  " JOIN " << temp_channels.at(i);
	std::string::size_type position = full_command.find(":");
	if (position == std::string::npos)
		ss << " :" << std::endl;
	else
	{
		ss << " ";
		ss << full_command.substr(position, JOINLEN) << std::endl;
	}
	_serverSettings.channels.at(_serverSettings.channels.size() - 1).sendToAllChannelMembers(ss.str(), _serverSettings.debug);
	ss.str("");

	ss << ":"; // Print topic
	ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
	ss << " " << RPL_TOPIC << " " << ToolFunctions::findNickName(socket, _serverSettings.clients);
	ss << " " << temp_channels.at(i) << " :";
	ss << _serverSettings.channels.at(_serverSettings.channels.size() - 1).getTopic() << std::endl;
	Server::sendToOneClient(socket, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Join::_inviteOnlyErrorHelper(	const int 								&socket,
								const std::vector<Channel>::size_type 	&k,
								t_server_mode 							&_serverSettings)
{
	std::stringstream ss;
	ss << _serverSettings.channels.at(k).getChannelName();
	ss << " :Cannot join channel (+i)";
	Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_INVITEONLYCHAN, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Join::_keyMatchesHelper(const int 								&socket,
						const std::string						&full_command,
						const std::vector<Channel>::size_type	&k,
						t_server_mode 							&_serverSettings)
{
	_serverSettings.channels.at(k).addToChannel(socket);
	std::stringstream ss;
	ss << ":"; // Join message
	ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
	ss << " JOIN " << _serverSettings.channels.at(k).getChannelName();
	std::string::size_type position = full_command.find(":");
	if (position == std::string::npos)
		ss << " :" << std::endl;
	else
	{
		ss << " ";
		ss << full_command.substr(position, JOINLEN) << std::endl;
	}
	_serverSettings.channels.at(_serverSettings.channels.size() - 1).sendToAllChannelMembers(ss.str(), _serverSettings.debug);
	ss.str("");

	ss << ":"; // Topic message
	ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
	ss << " " << RPL_TOPIC << " " << ToolFunctions::findNickName(socket, _serverSettings.clients);
	ss << " " << _serverSettings.channels.at(k).getChannelName() << " :";
	ss << _serverSettings.channels.at(k).getTopic() << std::endl;
	Server::sendToOneClient(socket, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Join::_keyDoesNotMatchErrorHelper(	const int 								&socket,
								const std::vector<Channel>::size_type	&k,
								t_server_mode							&_serverSettings)
{
	std::stringstream ss;
	ss << _serverSettings.channels.at(k).getChannelName();
	ss << " :Cannot join channel (+k)";
	Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_BADCHANNELKEY, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Join::_clientDoesNotProvideKeyErrorHelper(	const int 								&socket,
											const std::vector<Channel>::size_type	&k,
											const t_server_mode						&_serverSettings)
{
	std::stringstream ss;
	ss << _serverSettings.channels.at(k).getChannelName();
	ss << " :Cannot join channel (+k)";
	Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_BADCHANNELKEY, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Join::_channelDoesNotHaveKeyHelper(	const int 								&socket,
									const std::vector<Channel>::size_type	&k,
									const std::string						&full_command,
									t_server_mode							&_serverSettings)
{
	_serverSettings.channels.at(k).addToChannel(socket);
	std::stringstream ss;
	ss << ":"; // Join message
	ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
	ss << " JOIN " << _serverSettings.channels.at(k).getChannelName();
	std::string::size_type position = full_command.find(":");
	if (position == std::string::npos)
		ss << " :" << std::endl;
	else
	{
		ss << " ";
		ss << full_command.substr(position) << std::endl;
	}
	_serverSettings.channels.at(_serverSettings.channels.size() - 1).sendToAllChannelMembers(ss.str(), _serverSettings.debug);
	ss.str("");

	ss << ":"; // Topic message
	ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
	ss << " " << RPL_TOPIC << " " << ToolFunctions::findNickName(socket, _serverSettings.clients);
	ss << " " << _serverSettings.channels.at(k).getChannelName() << " :";
	ss << _serverSettings.channels.at(k).getTopic() << std::endl;
	Server::sendToOneClient(socket, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Join::_channelIsFullErrorHelper(const int 								&socket,
								const std::vector<Channel>::size_type	&k,
								const t_server_mode						&_serverSettings)
{
	std::stringstream ss;
	ss << _serverSettings.channels.at(k).getChannelName();
	ss << " :Cannot join channel (+l)";
	Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_BADCHANNELKEY, ss.str(), _serverSettings.debug);
	ss.str("");
}
