/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:35:45 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/10 20:33:39 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Privmsg.hpp"
#include "ToolFunctions.hpp"

void Privmsg::privmsgcmd(int socket, std::string full_command, t_server_mode &_serverSettings)
{
	std::vector<std::string> commandParts;
	//std::vector<std::string> tempChannels;
	//std::vector<std::string> tempKeys;
	ToolFunctions::_split_command_in_parts(full_command, commandParts);

	std::cout << "privmsgcmd entered" << std::endl;

	if (commandParts.size() < 3 || commandParts.at(2).at(0) != ':')
	{
		std::cout << "privmsgcmd should not come to here" << std::endl;
		return ;
	}
	if (commandParts.at(1).at(0) == '#') // Target is Channel
	{
		if(!Server::doesChannelExist(commandParts.at(1), _serverSettings.channels)) // Channel does not exist
		{
			std::stringstream ss;
			ss << commandParts.at(1);
			ss << " :No such channel" << std::endl;
			Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_NOSUCHCHANNEL, ss.str());
			ss.str("");
			return ;
		}
		for (std::vector<Channel>::size_type i = 0; i < _serverSettings.channels.size(); ++i) // Channel exist
		{
			if (_serverSettings.channels.at(i).getChannelName() == commandParts.at(1))
			{
				if (_serverSettings.channels.at(i).isOnChannel(socket)) // Sender on channel
				{
					std::string message;
					std::string::size_type position = full_command.find(":");
					if (position != std::string::npos)
						message = full_command.substr(position);
					std::stringstream ss;
					ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients) << "!localhost" << " PRIVMSG ";
					ss << commandParts.at(1) << " " << message << std::endl;
					_serverSettings.channels.at(i).sendToAllChannelMembers(ss.str());
					ss.str("");
					return ;
				}else // Sender not on channel
				{
					std::stringstream ss;
					ss << commandParts.at(1);
					ss << " :Cannot send to channel" << std::endl;
					Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_CANNOTSENDTOCHAN, ss.str());
					ss.str("");
					return ;
				}
				break ;
			}
			
		}
	}else // Target is a user
	{
		int targetSocket = ToolFunctions::_findSocket(commandParts.at(1), _serverSettings.clients);
		if (targetSocket == 0) // Nick not found
		{
			std::stringstream ss;
			ss << commandParts.at(1);
			ss << ":No such nick" << std::endl;
			Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_NOSUCHNICK, ss.str());
			ss.str("");
			return ;
		}
		std::string message;
		std::string::size_type position = full_command.find(":");
		if (position != std::string::npos)
			message = full_command.substr(position);
		std::stringstream ss;
		ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients) << "!localhost" << " PRIVMSG ";
		ss << commandParts.at(1) << " " << message << std::endl;
		Server::sendToOneClient(targetSocket, ss.str());
		ss.str("");
	}
}
