/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:07:55 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/05 20:15:29 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Join.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
#include "../includes/ToolFunctions.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

void Join::joincmd(int socket, std::string full_command, t_server_mode	&_serverSettings)
{
	std::vector<std::string> commandParts;
	std::vector<std::string> tempChannels;
	std::vector<std::string> tempKeys;
	ToolFunctions::_split_command_in_parts(full_command, commandParts);

	std::cout << "joincmd entered" << std::endl;

	switch(commandParts.size())
	{
		case 0:
			std::cout << "joincmd should not come to here" << std::endl;
			break;

		case 1:
			std::cout << "joincmd should not come to here" << std::endl;
			break;

		case 2:
			if (commandParts.at(1) == ":")	// For compatibility with irssi
				return ;					// ":" Is not in a Standard
			if (commandParts.at(1) == "0") // Part from all channels
			{
				for (std::vector<std::string>::size_type i = 0; i < _serverSettings.channels.size(); ++i)
				{
					if (_serverSettings.channels.at(i).isOnChannel(socket))
					{
						_serverSettings.channels.at(i).partFromChannel(socket);
						std::stringstream ss;
						ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients);
						ss << "!" << "localhost" << " PART " << _serverSettings.channels.at(i).getChannelName() << " :0";
						_serverSettings.channels.at(i).sendToAllChannelMembers(ss.str());
						ss.clear();
					}
				}
			}
			ToolFunctions::_parse_into_parts(commandParts, 1, tempChannels);
			for (std::vector<std::string>::size_type i = 0; i < tempChannels.size(); ++i)
			{
				for (std::vector<Channel>::size_type k = 0; k < _serverSettings.channels.size(); ++k)
				{
					if (tempChannels.at(i) == _serverSettings.channels.at(k).getChannelName())
					{
						std::cout << "Join channel exist" << std::endl;
						if (_serverSettings.channels.at(k).isOnChannel(socket))
						{
							// Check with laptop what happens if already on channel?
							std::stringstream ss;
							ss << _serverSettings.channels.at(k).getChannelName();
							ss << " :is already on channel";
							Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_USERONCHANNEL, ss.str());
							ss.clear();
							break;
						}else if (_serverSettings.channels.at(k).isInviteOnly() && !_serverSettings.channels.at(k).isClientInvited(socket))
						{
							std::stringstream ss;
							ss << _serverSettings.channels.at(k).getChannelName();
							ss << " :Cannot join channel (+i)";
							Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_INVITEONLYCHAN, ss.str());
							ss.clear();
							break;
						} else
						{
							_serverSettings.channels.at(k).addToChannel(socket);
							// Remember to add channels joined in client
							std::stringstream ss;
							ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients);
							ss << "!" << "localhost" <<  " JOIN" << " :" << _serverSettings.channels.at(k).getChannelName();
							Server::sendToOneClient(socket, ss.str());
							ss.clear();

							// Print Channel members with or without "@" that means ops or not
							const std::vector<int> &clients = _serverSettings.channels.at(k).returnChannelMembers();
							//int firstClient = 0;
							for (std::vector<int>::size_type i = 0; i < clients.size(); ++i)
							{
								std::cout << "JOIN else for loop" << std::endl;
								//if (firstClient == 1)
								//	ss << " ";
								if (_serverSettings.channels.at(k).hasOps(clients.at(i)))
									ss << ":@";
								ss  << ToolFunctions::_findNickName(clients.at(i), _serverSettings.clients);
								//firstClient  = 1;
								Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), RPL_NAMREPLY, ss.str());
								ss.clear();
							}
							//Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), RPL_NAMREPLY, ss.str());
							//ss.clear();
							std::cout << "Test printing before end of names list." << std::endl;
							Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), RPL_ENDOFNAMES, ":End of NAMES list.");
							ss.clear();
							break;
						}
					}
				}
				// Channel does not exist so create a new one
				if (!Server::doesChannelExist(tempChannels.at(i), _serverSettings.channels))
					_serverSettings.channels.push_back(Channel(tempChannels.at(i), socket));
			}
			break;
		
		case 3:
			ToolFunctions::_parse_into_parts(commandParts, 1, tempChannels);
			ToolFunctions::_parse_into_parts(commandParts, 2, tempKeys);
			// Should we check if key checks out if there is no key 
			break;
		
		default:
			std::cout << "joincmd should not come to here" << std::endl;
			break;
	}
}
