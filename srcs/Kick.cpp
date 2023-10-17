/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:35:34 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/17 19:14:31 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kick.hpp"
#include "ToolFunctions.hpp"

void Kick::kickcmd(int socket, std::string full_command, t_server_mode	&_serverSettings)
{
	t_kickcmd_data data;

	ToolFunctions::_split_command_in_parts(full_command, data.commandParts);
	if (data.commandParts.size() < 3)
		return ; // Needs allways atleast 3 parts

	data.socket = socket;
	data.fullCommand = full_command;
	ToolFunctions::_parse_into_parts(data.commandParts, 1, data.tempChannels);
	ToolFunctions::_parse_into_parts(data.commandParts, 2, data.tempUsers);

	std::cout << "kickcmd entered" << std::endl;
	
	if (data.tempChannels.size() >= 1 && data.tempUsers.size() >= 1) // handle all channels and users at the same time against rfc2812
	{
		for (std::vector<std::string>::size_type j = 0; j < data.tempChannels.size(); ++j)
		{
			if (!ToolFunctions::doesChannelExistWithName(data.tempChannels.at(j), _serverSettings.channels))
			{
				_printDoesChannelExistError(data, j, _serverSettings);
				continue ;
			}
			for (std::vector<Channel>::size_type i = 0; i < _serverSettings.channels.size(); ++i) // Channel exist
			{
				if (_serverSettings.channels.at(i).getChannelName() == data.tempChannels.at(j))
				{
					if (!_serverSettings.channels.at(i).hasOps(data.socket))
					{
						_printYoureNotChannelOperatorError(data, j, _serverSettings);
						break ;
					}
					if (_goThroughTempUsers(data, j, i, _serverSettings))
						break;
					break ;
				}
			}
		}
	}else
	{
		std::cout << "KICKCMD SYNTAX ERROR" << std::endl;
	}
}

int Kick::_returnClientSocket(std::string nick, t_server_mode &_serverSettings)
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (_serverSettings.clients[i].getNick() == nick)
			return (_serverSettings.clients[i].getSocket());
	}
	return (0);
}

void Kick::_printDoesChannelExistError(t_kickcmd_data &data, std::vector<std::string>::size_type &j, t_server_mode &_serverSettings)
{
	std::stringstream ss;
	ss << data.tempChannels.at(j);
	ss << " :No such channel" << std::endl;
	Server::sendAnswer(data.socket, ToolFunctions::_findNickName(data.socket, _serverSettings.clients), ERR_NOSUCHCHANNEL, ss.str());
	ss.str("");
}

void Kick::_printYoureNotChannelOperatorError(t_kickcmd_data &data, std::vector<std::string>::size_type &j, t_server_mode &_serverSettings)
{
	std::stringstream ss;
	ss << data.tempChannels.at(j);
	ss << " :You're not channel operator" << std::endl;
	Server::sendAnswer(data.socket, ToolFunctions::_findNickName(data.socket, _serverSettings.clients), ERR_CHANOPRIVSNEEDED, ss.str());
	ss.str("");
}

void Kick::_printUserIsNotOnThatChannelError(t_kickcmd_data &data, std::vector<std::string>::size_type &j, t_server_mode &_serverSettings)
{
	std::stringstream ss;
	ss << data.tempChannels.at(j);
	ss << " :User is not on that channel" << std::endl;
	Server::sendAnswer(data.socket, ToolFunctions::_findNickName(data.socket, _serverSettings.clients), ERR_USERNOTINCHANNEL, ss.str());
	ss.str("");
}

void Kick::_kickUserFromChannel(t_kickcmd_data &data, std::vector<Channel>::size_type &i, std::vector<std::string>::size_type &k, t_server_mode &_serverSettings)
{
	_serverSettings.channels.at(i).partFromChannel(_returnClientSocket(data.tempUsers.at(k), _serverSettings));
	_serverSettings.channels.at(i).setNewOpIfNoOp();
	std::stringstream ss;
	ss << ":" << ToolFunctions::_findNickName(data.socket, _serverSettings.clients);
	ss << "!" << "localhost" <<  " KICK " << _serverSettings.channels.at(i).getChannelName();
	ss << " " << data.tempUsers.at(k);
	
	if (data.commandParts.size() >= 4)
	{
		std::string::size_type position = data.fullCommand.find(":");
		if (position != std::string::npos)
			ss << " " << data.fullCommand.substr(position);
	}
	ss << std::endl;
	Server::sendToOneClient(data.socket, ss.str());
	Server::sendToOneClient(_returnClientSocket(data.tempUsers.at(k), _serverSettings), ss.str());
	ss.str("");
	if (_serverSettings.channels.at(i).howManyMembersOnChannel() == 0)
		_serverSettings.channels.erase(_serverSettings.channels.begin() + i--);
}

int Kick::_goThroughTempUsers(t_kickcmd_data &data, std::vector<std::string>::size_type &j, std::vector<Channel>::size_type &i, t_server_mode &_serverSettings)
{
	for (std::vector<std::string>::size_type k = 0; k < data.tempUsers.size(); ++k)
	{
		if (!ToolFunctions::doesChannelExistWithName(data.tempChannels.at(j), _serverSettings.channels)) // If last user is deleted channel might not exist
		{
			_printDoesChannelExistError(data, j, _serverSettings);
			return (1);
		}
		if (!_serverSettings.channels.at(i).isOnChannel(_returnClientSocket(data.tempUsers.at(k), _serverSettings)))
		{
			_printUserIsNotOnThatChannelError(data, j, _serverSettings);
			continue ;
		}
		_kickUserFromChannel(data, i, k, _serverSettings);
	}
	return (0);
}
