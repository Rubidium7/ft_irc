/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:07:55 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/02 19:57:35 by tpoho            ###   ########.fr       */
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
	std::vector<std::string> command_parts;
	std::vector<std::string> temp_channels;
	std::vector<std::string> temp_keys;
	ToolFunctions::_split_command_in_parts(full_command, command_parts);

	std::cout << "joincmd entered" << std::endl;

	switch(command_parts.size())
	{
		case 0:
			std::cout << "joincmd should not come to here" << std::endl;
			break;

		case 1:
			std::cout << "joincmd should not come to here" << std::endl;
			break;

		case 2:
			if (command_parts.at(1) == ":")
				return ;
			ToolFunctions::_parse_into_parts(command_parts, 1, temp_channels);
			for (std::vector<std::string>::size_type i = 0; i < temp_channels.size(); ++i)
			{
				for (std::vector<Channel>::size_type k = 0; k < _serverSettings.channels.size(); ++k)
				{
					if (temp_channels.at(i) == _serverSettings.channels.at(k).getChannelName())
					{
						if (_serverSettings.channels.at(k).isOnChannel(socket))
						{
							// Check with laptop what happens if already on channel?
							//Server::sendAnswer(socket, _findNickName(socket, _serverSettings.clients), 666, "Already on channel")
							break;
						}else
						{
							_serverSettings.channels.at(k).addToChannel(socket);
							// Remember to add channels joined in client
							// change client vector 
							//Server::sendAnswer(socket, _findNickName(socket, _serverSettings.clients), 666, "Joined Channel")
							break;
						}
					}
				}
				// Channel does not exist so create a new one
				Channel temp(temp_channels.at(i), socket);
				_serverSettings.channels.push_back(Channel(temp_channels.at(i), socket));
			}
			break;
		
		case 3:
			ToolFunctions::_parse_into_parts(command_parts, 1, temp_channels);
			ToolFunctions::_parse_into_parts(command_parts, 2, temp_keys);
			// Should we check if key checks out if there is no key 
			break;
		
		default:
			std::cout << "joincmd should not come to here" << std::endl;
			break;
	}
}
