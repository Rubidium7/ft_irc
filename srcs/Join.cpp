/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:07:55 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/09 17:37:23 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"
#include "Part.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "ToolFunctions.hpp"
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
			if (commandParts.at(1) == ":")	// For compatibility with irssi, ":" Is not in a Standard
				return ;
			if (commandParts.at(1) == "0") // Part from all channels
			{
				Part::partFromAllChannels(socket, _serverSettings);
				return ;
			}

		case 3:
			
			ToolFunctions::_parse_into_parts(commandParts, 1, tempChannels);
			if (commandParts.size() == 3)
				ToolFunctions::_parse_into_parts(commandParts, 2, tempKeys);
			for (std::vector<std::string>::size_type i = 0; i < tempChannels.size(); ++i)
			{
				// Channel does not exist so create a new one
				if (!Server::doesChannelExist(tempChannels.at(i), _serverSettings.channels))
				{
					_serverSettings.channels.push_back(Channel(tempChannels.at(i), socket));
					_serverSettings.channels.at(_serverSettings.channels.size() - 1).giveOps(socket);
					std::stringstream ss;
					ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients);
					ss << "!" << "localhost" <<  " JOIN" << " :" << tempChannels.at(i) << std::endl;
					Server::sendToOneClient(socket, ss.str());
					ss.clear();
					continue ;
				}
				for (std::vector<Channel>::size_type k = 0; k < _serverSettings.channels.size(); ++k)
				{
					if (tempChannels.at(i) == _serverSettings.channels.at(k).getChannelName())
					{
						if (_serverSettings.channels.at(k).isOnChannel(socket))
						{
							// On purpose do nothing if already on a channel
							// Different servers react differently
							// Some do nothing some part and rejoin user
							// There is no standard way to handle this
							break;
						}else if (_serverSettings.channels.at(k).isInviteOnly() && !_serverSettings.channels.at(k).isClientInvited(socket))
						{
							std::stringstream ss;
							ss << _serverSettings.channels.at(k).getChannelName();
							ss << " :Cannot join channel (+i)" << std::endl;
							Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_INVITEONLYCHAN, ss.str());
							ss.clear();
							break;
						} else
						{
							if (_serverSettings.channels.at(k).isThereKey()) // Does Channel have key set (password)
							{
								if (i < tempKeys.size()) 
								{ // Client has given key
									if (_serverSettings.channels.at(k).doesKeyMatch(tempKeys.at(i)))
									{ // Key matches
										_serverSettings.channels.at(k).addToChannel(socket);
										std::stringstream ss;
										ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients);
										ss << "!" << "localhost" <<  " JOIN" << " :" << _serverSettings.channels.at(k).getChannelName() << std::endl;
										Server::sendToOneClient(socket, ss.str());
										ss.clear();
										break;
									}else // Key does not match
									{
										std::stringstream ss;
										ss << _serverSettings.channels.at(k).getChannelName();
										ss << " :Cannot join channel (+k)" << std::endl;
										Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_BADCHANNELKEY, ss.str());
										ss.clear();
										break;
									}
								}else //Client does not provide key
								{
									std::stringstream ss;
									ss << _serverSettings.channels.at(k).getChannelName();
									ss << " :Cannot join channel (+k)" << std::endl;
									Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_BADCHANNELKEY, ss.str());
									ss.clear();
									break;
								}
							}
							_serverSettings.channels.at(k).addToChannel(socket);
							std::stringstream ss;
							ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients);
							ss << "!" << "localhost" <<  " JOIN" << " :" << _serverSettings.channels.at(k).getChannelName() << std::endl;
							Server::sendToOneClient(socket, ss.str());
							ss.clear();
							break;
						}
					}
				}
			}
			break;
		
		default:
			std::cout << "joincmd should not come to here" << std::endl;
			break;
	}
}
