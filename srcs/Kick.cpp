
#include "Kick.hpp"
#include "ToolFunctions.hpp"

void
Kick::kickCommand(	int				socket,
					std::string		full_command,
					 t_server_mode	&_serverSettings)
{
	t_kickcmd_data data;

	ToolFunctions::splitCommandInParts(full_command, data.command_parts);
	data.socket = socket;
	data.full_command = full_command;
	ToolFunctions::parseIntoParts(data.command_parts, 1, data.temp_channels);
	ToolFunctions::parseIntoParts(data.command_parts, 2, data.temp_users);

	for (std::vector<std::string>::size_type j = 0; j < data.temp_channels.size(); ++j) // Handle all channels and users at the same time against rfc2812 rule
	{																					// Otherwise irssi will not work, pdf says we can modify behavior of server to fit client
		if (!ToolFunctions::doesChannelExistWithName(data.temp_channels.at(j), _serverSettings.channels)) // Channel does not exist
		{
			_printDoesChannelExistError(data, j, _serverSettings);
			continue ;
		}
		for (std::vector<Channel>::size_type i = 0; i < _serverSettings.channels.size(); ++i) // Channel exist
		{
			if (_serverSettings.channels.at(i).getChannelName() == data.temp_channels.at(j)) // Name of channel maches
			{
				if (!_serverSettings.channels.at(i).hasOps(data.socket)) // Client does not have ops
				{
					_printYoureNotChannelOperatorError(data, j, _serverSettings);
					break ;
				}
				_goThroughTempUsersLoopHelper(data, j, i, _serverSettings); // Client has ops
				break ;
			}
		}
	}
}

void
Kick::_printDoesChannelExistError(	const t_kickcmd_data 				&data,
									std::vector<std::string>::size_type &j,
									t_server_mode						&_serverSettings)
{
	std::stringstream ss;
	ss << data.temp_channels.at(j);
	ss << " :No such channel";
	Server::sendAnswer(data.socket, ToolFunctions::findNickName(data.socket, _serverSettings.clients), ERR_NOSUCHCHANNEL, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Kick::_printYoureNotChannelOperatorError(	t_kickcmd_data						&data,
											std::vector<std::string>::size_type &j,
											t_server_mode						&_serverSettings)
{
	std::stringstream ss;
	ss << data.temp_channels.at(j);
	ss << " :You're not channel operator";
	Server::sendAnswer(data.socket, ToolFunctions::findNickName(data.socket, _serverSettings.clients), ERR_CHANOPRIVSNEEDED, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Kick::_printUserIsNotOnThatChannelError(const t_kickcmd_data				&data,
										std::vector<std::string>::size_type &j,
										t_server_mode						&_serverSettings)
{
	std::stringstream ss;
	ss << data.temp_channels.at(j);
	ss << " :User is not on that channel";
	Server::sendAnswer(data.socket, ToolFunctions::findNickName(data.socket, _serverSettings.clients), ERR_USERNOTINCHANNEL, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Kick::_kickUserFromChannel(	const t_kickcmd_data				&data,
							std::vector<Channel>::size_type		&i,
							std::vector<std::string>::size_type &k,
							t_server_mode						&_serverSettings)
{
	std::stringstream ss;
	ss << ":"; // Kick message
	ss << USER_ID(ToolFunctions::findNickName(data.socket, _serverSettings.clients), ToolFunctions::findUserName(data.socket, _serverSettings.clients));
	ss << " KICK " << _serverSettings.channels.at(i).getChannelName();
	ss << " " << data.temp_users.at(k);
	std::string::size_type position = data.full_command.find(":");
	if (position != std::string::npos)
			ss << " " << data.full_command.substr(position, KICKLEN);
	ss << std::endl;
	_serverSettings.channels.at(i).sendToAllChannelMembers(ss.str(), _serverSettings.debug);

	ss.str(""); // Actual removal of user from channel
	_serverSettings.channels.at(i).partFromChannel(ToolFunctions::findSocketForClientFromName(data.temp_users.at(k), _serverSettings.clients));
	_serverSettings.channels.at(i).setNewOpIfNoOp();
	if (_serverSettings.channels.at(i).howManyMembersOnChannel() == 0)
		_serverSettings.channels.erase(_serverSettings.channels.begin() + i--);
}

void
Kick::_goThroughTempUsersLoopHelper(const t_kickcmd_data 				&data,
									std::vector<std::string>::size_type &j,
									std::vector<Channel>::size_type 	&i,
									t_server_mode 						&_serverSettings)
{
	for (std::vector<std::string>::size_type temp_user_index = 0; temp_user_index < data.temp_users.size(); ++temp_user_index) // Go through all candidate users
	{
		if (!ToolFunctions::doesChannelExistWithName(data.temp_channels.at(j), _serverSettings.channels)) // If last user is deleted channel might not exist
		{
			_printDoesChannelExistError(data, j, _serverSettings);
			return ;
		}
		if (!_serverSettings.channels.at(i).isOnChannel(ToolFunctions::findSocketForClientFromName(data.temp_users.at(temp_user_index), _serverSettings.clients))) // User not in channel
		{
			_printUserIsNotOnThatChannelError(data, j, _serverSettings);
			continue ;
		}
		_kickUserFromChannel(data, i, temp_user_index, _serverSettings);
	}
}
