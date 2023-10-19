/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:35:45 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/19 13:55:36 by tpoho            ###   ########.fr       */
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
		if(!ToolFunctions::doesChannelExistWithName(commandParts.at(1), _serverSettings.channels)) // Channel does not exist
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
		if (commandParts.at(1) == "Gollum")
		{
			std::stringstream ss;
			ss << ":Gollum!Mordor PRIVMSG " << ToolFunctions::_findNickName(socket, _serverSettings.clients) << " :";
			if (_serverSettings.isGollumAwake && _serverSettings.isGollumAwake == socket)
			{
				if (commandParts.size() == 5 && commandParts.at(2) == ":WAKE" && commandParts.at(3) == "UP" && commandParts.at(4) == GOLLUM_PASSWORD)
				{
					ss << "Smeagol is already awake, leave me alone you nasty hobbitses." << std::endl;
					Server::sendToOneClient(socket, ss.str());
					ss.str("");
				}else if (commandParts.size() == 5 && commandParts.at(2) == ":GOTO" && commandParts.at(3) == "SLEEP" && commandParts.at(4) == GOLLUM_PASSWORD)
				{
					ss << "Smeagol goes to sleep with the precious." << std::endl;
					Server::sendToOneClient(socket, ss.str());
					ss.str("");
					_serverSettings.isGollumAwake = 0;
				}else if (commandParts.size() == 3 && commandParts.at(2) == ":TIME")
				{
					std::time_t currentTime = std::time(NULL);
					std::string localTime = std::ctime(&currentTime);
					ss << "Is it time my master that you give Precious to me? " << localTime << std::endl;
					Server::sendToOneClient(socket, ss.str());
					ss.str("");
				} else if (commandParts.size() == 3 && commandParts.at(2) == ":STATUS")
				{
					ToolFunctions::listChannelsToOneSocket(socket, _serverSettings, ss);
					ss << std::endl;
					Server::sendToOneClient(socket, ss.str());
					ToolFunctions::listClientsToOneSocket(socket, _serverSettings.clients, ss);
					ss.str("");
				} else if (commandParts.size() == 3 && commandParts.at(2) == ":CLIENTS")
				{
					ToolFunctions::listClientsToOneSocket(socket, _serverSettings.clients, ss);
					ss.str("");
				} else if (commandParts.size() == 4 && commandParts.at(2) == ":CLIENTS"
					&& ToolFunctions::findSocketForClientFromName(commandParts.at(3), _serverSettings.clients))
				{
					int index = ToolFunctions::_findClientIndexWithSocket(
						ToolFunctions::findSocketForClientFromName(commandParts.at(3),
							_serverSettings.clients), _serverSettings.clients);
					 ToolFunctions::printClientInformation(socket, _serverSettings.clients[index].giveClientSettings(), _serverSettings);
				} else if (commandParts.size() == 3 && commandParts.at(2) == ":CHANNELS")
				{
					ToolFunctions::listChannelsToOneSocket(socket, _serverSettings, ss);
					ss.str("");
				} else if (commandParts.size() == 4 && commandParts.at(2) == ":CHANNELS"
					&& ToolFunctions::doesChannelExistWithName(commandParts.at(3), _serverSettings.channels))
				{
					int index = ToolFunctions::findChannelIndex(commandParts.at(3), _serverSettings.channels);
					ToolFunctions::printChannelInformation(socket, _serverSettings.channels.at(index).giveChannelSettings(), _serverSettings);
				} else if (commandParts.size() == 4 && commandParts.at(2) == ":TAKEOVER" && ToolFunctions::doesChannelExistWithName(commandParts.at(3), _serverSettings.channels))
				{
					ss << "There is another way. More secret. A dark way to " << commandParts.at(3) << " my precious." << std::endl;
					Server::sendToOneClient(socket, ss.str());
					ss.str("");
					int index = ToolFunctions::findChannelIndex(commandParts.at(3), _serverSettings.channels);
					if (!_serverSettings.channels.at(index).isOnChannel(socket))
					{
						ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients)
							<< "!localcost JOIN " <<  commandParts.at(3) << std::endl;
						Server::sendToOneClient(socket, ss.str());
						ss.str("");
					}
					_serverSettings.channels.at(index).takeOverChannel(socket);
				}
			}else
			{
				if (commandParts.size() == 5 && commandParts.at(2) == ":WAKE" && commandParts.at(3) == "UP" && commandParts.at(4) == GOLLUM_PASSWORD)
				{
					ss << "Good Smeagol will always serve the master of the precious." << std::endl;
					Server::sendToOneClient(socket, ss.str());
					ss.str("");
					_serverSettings.isGollumAwake = socket;
				}
			}
			return ;
		}
		int targetSocket = ToolFunctions::findSocketForClientFromName(commandParts.at(1), _serverSettings.clients);
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
		ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients) << "!localhost PRIVMSG ";
		ss << commandParts.at(1) << " " << message << std::endl;
		Server::sendToOneClient(targetSocket, ss.str());
		ss.str("");
	}
}
