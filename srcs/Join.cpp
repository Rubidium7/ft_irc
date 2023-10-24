/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:07:55 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/24 15:37:53 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"
#include "Part.hpp"
#include "ToolFunctions.hpp"

void
Join::joinCommand(	const int		&socket,
					std::string		full_command,
					t_server_mode 	&_serverSettings)
{
	std::vector<std::string> command_parts; // full_command is split into parts here
	std::vector<std::string> temp_channels; // command_parts.at(1) is split into here
	std::vector<std::string> temp_keys;		// command_parts.at(2) is split into here
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
				}else if (_serverSettings.channels.at(k).isInviteOnly() && !_serverSettings.channels.at(k).isClientInvited(socket)) // Invite only channel
				{
					_inviteOnlyErrorHelper(socket, k, _serverSettings); break;
				} else
				{
					if (_serverSettings.channels.at(k).isThereKey()) // Does Channel have key set (password)
					{
						if (i < temp_keys.size()) // Client provides key
						{
							if (_serverSettings.channels.at(k).doesKeyMatch(temp_keys.at(i))) // Key matches
							{
								_keyMatchesHelper(socket, full_command, k, _serverSettings); break ;
							}else // Key does not match
							{
								_keyDoesNotMatchHelper(socket, k, _serverSettings); break ;
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
									const std::vector<std::string>				&temp_channels,
									const std::vector<std::string>				&temp_keys,
									t_server_mode 								&_serverSettings)
{
	_serverSettings.channels.push_back(Channel(temp_channels.at(i), socket)); // Create Channel
	_serverSettings.channels.at(_serverSettings.channels.size() - 1).giveOps(socket); //Give ops to only user
	if (i < temp_keys.size())
		_serverSettings.channels.at(_serverSettings.channels.size() - 1).setKey(temp_keys.at(i));

	std::stringstream ss;
	ss << ":" << ToolFunctions::findNickName(socket, _serverSettings.clients); // Join message
	ss << "!" << "localhost" <<  " JOIN " << temp_channels.at(i);
	std::string::size_type position = full_command.find(":");
	if (position == std::string::npos)
		ss << " :" << std::endl;
	else
	{
		ss << " ";
		ss << full_command.substr(position) << std::endl;
	}
	_serverSettings.channels.at(_serverSettings.channels.size() - 1).sendToAllChannelMembers(ss.str());
	ss.str("");
			
	ss << ":localhost " << RPL_TOPIC << " " << ToolFunctions::findNickName(socket, _serverSettings.clients); // Print topic
	ss << " " << temp_channels.at(i) << " :";
	ss << _serverSettings.channels.at(_serverSettings.channels.size() - 1).getTopic() << std::endl;
	Server::sendToOneClient(socket, ss.str());
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
	Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_INVITEONLYCHAN, ss.str());
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
	ss << ":" << ToolFunctions::findNickName(socket, _serverSettings.clients);
	ss << "!" << "localhost" <<  " JOIN " << _serverSettings.channels.at(k).getChannelName();
	std::string::size_type position = full_command.find(":");
	if (position == std::string::npos)
		ss << " :" << std::endl;
	else
	{
		ss << " ";
		ss << full_command.substr(position) << std::endl;
	}
	_serverSettings.channels.at(_serverSettings.channels.size() - 1).sendToAllChannelMembers(ss.str());
	ss.str("");
}

void
Join::_keyDoesNotMatchHelper(	const int 								&socket,
								const std::vector<Channel>::size_type	&k,
								t_server_mode							&_serverSettings)
{
	std::stringstream ss;
	ss << _serverSettings.channels.at(k).getChannelName();
	ss << " :Cannot join channel (+k)";
	Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_BADCHANNELKEY, ss.str());
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
	Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_BADCHANNELKEY, ss.str());
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
	ss << ":" << ToolFunctions::findNickName(socket, _serverSettings.clients);
	ss << "!" << "localhost" <<  " JOIN " << _serverSettings.channels.at(k).getChannelName();
	std::string::size_type position = full_command.find(":");
	if (position == std::string::npos)
		ss << " :" << std::endl;
	else
	{
		ss << " ";
		ss << full_command.substr(position) << std::endl;
	}
	_serverSettings.channels.at(_serverSettings.channels.size() - 1).sendToAllChannelMembers(ss.str());
	ss.str("");
						
	ss << ":localhost " << RPL_TOPIC << " " << ToolFunctions::findNickName(socket, _serverSettings.clients);
	ss << " " << _serverSettings.channels.at(k).getChannelName() << " :";
	ss << _serverSettings.channels.at(k).getTopic() << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");
}
