
#include "ToolFunctions.hpp"
#include "Part.hpp"

void
Part::partCommand(	int 			socket,
					std::string 	full_command,
					t_server_mode	&_serverSettings)
{
	std::vector<std::string> command_parts;
	std::vector<std::string> temp_channels;
	ToolFunctions::splitCommandInParts(full_command, command_parts);

	ToolFunctions::parseIntoParts(command_parts, 1, temp_channels);
	for (std::vector<std::string>::size_type i = 0; i < temp_channels.size(); ++i)
	{
		if (!ToolFunctions::doesChannelExistWithName(temp_channels.at(i), _serverSettings.channels))
			Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_NOSUCHCHANNEL, ":No such channel", _serverSettings.debug);
		for (std::vector<Channel>::size_type k = 0; k < _serverSettings.channels.size(); ++k)
		{
			if (temp_channels.at(i) == _serverSettings.channels.at(k).getChannelName())
			{
				if (_serverSettings.channels.at(k).isOnChannel(socket)) // Client on channel
					_partCommandClientOnChannelHelper(socket, full_command, k, _serverSettings);
				else // Client not on channel
					Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_NOTONCHANNEL, ":Not on that channel", _serverSettings.debug);
				break ;
			}
		}
	}
}

void
Part::partFromAllChannels(	int 			socket,
							t_server_mode	&_serverSettings)
{
	for (std::vector<std::string>::size_type i = 0; i < _serverSettings.channels.size(); ++i)
	{
		if (_serverSettings.channels.at(i).isOnChannel(socket)) // Client on channel
		{
			std::stringstream ss;
			ss << ":"; // Part message
			ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
			ss << " PART " << _serverSettings.channels.at(i).getChannelName() << " :" << std::endl;
			_serverSettings.channels.at(i).sendToAllChannelMembers(ss.str(), _serverSettings.debug);
			ss.str("");

			_serverSettings.channels.at(i).partFromChannel(socket); // Actual removal from channel
			_serverSettings.channels.at(i).setNewOpIfNoOp();
			if (_serverSettings.channels.at(i).howManyMembersOnChannel() == 0) // If channel has no members the channel is removed
				_serverSettings.channels.erase(_serverSettings.channels.begin() + i--); // i-- because all channels move one step back
		}
	}
}

void
Part::_partCommandClientOnChannelHelper(const int 						&socket,
										const std::string 				full_command,
										std::vector<Channel>::size_type &k,
										t_server_mode 					&_serverSettings)
{
	std::stringstream ss;
	ss << ":"; // Part message
	ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
	ss << " PART " << _serverSettings.channels.at(k).getChannelName();
	std::string::size_type position = full_command.find(":");
	if (position == std::string::npos)
		ss << " :" << std::endl;
	else
	{
		ss << " ";
		ss << full_command.substr(position, PARTLEN)  << std::endl;
	}
	_serverSettings.channels.at(k).sendToAllChannelMembers(ss.str(), _serverSettings.debug);
	_serverSettings.channels.at(k).partFromChannel(socket);
	_serverSettings.channels.at(k).setNewOpIfNoOp();
	ss.str("");
	if (_serverSettings.channels.at(k).howManyMembersOnChannel() == 0) // If channels has no users it will be removed
		_serverSettings.channels.erase(_serverSettings.channels.begin() + k--); // k-- because all channels move one step back
}
