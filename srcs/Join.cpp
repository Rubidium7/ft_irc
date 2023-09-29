/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:07:55 by tpoho             #+#    #+#             */
/*   Updated: 2023/09/29 20:07:31 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Join.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

void Join::joincmd(int socket, std::string full_command, t_server_mode	&_serverSettings)
{
	std::vector<std::string> command_parts;
	std::vector<std::string> temp_channels;
	std::vector<std::string> temp_keys;
	_split_command_in_parts(full_command, command_parts);

	std::cout << "joincmd entered" << std::endl;

	switch(command_parts.size())
	{
		case 2:
			if (command_parts.at(1) == ":")
				return ;
			_parse_into_parts(command_parts, 1, temp_channels);
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
						}else
						{
							_serverSettings.channels.at(k).addToChannel(socket);
							// Remember to add channels joined in client
							// change client vector 
							//Server::sendAnswer(socket, _findNickName(socket, _serverSettings.clients), 666, "Joined Channel")
						}
					}
				}	
			}

			// Channel does not exist
			break;
		
		case 3:
			_parse_into_parts(command_parts, 1, temp_channels);
			_parse_into_parts(command_parts, 2, temp_keys);
			break;
	}
}

void Join::_split_command_in_parts(const std::string &full_command, std::vector<std::string> &command_parts)
{
	std::stringstream	ss(full_command);
	std::string			part;
	while (ss >> part)
		command_parts.push_back(part);
}

void Join::_parse_into_parts(const std::vector<std::string> &command_parts, int part, std::vector<std::string> &temp_strings)
{
	std::stringstream ss(command_parts.at(part));
	std::string temp;
    while (std::getline(ss, temp, ','))
		temp_strings.push_back(temp);
	// Remove ',' from strings
	for (std::vector<std::string>::size_type i = 0; i < temp_strings.size(); ++i)
	{
		std::string::size_type position = temp_strings.at(i).find(',');
		if (position != std::string::npos)
		{
			temp_strings.at(i).erase(position, 1);
		}
	}
}

std::string	Join::_findNickName(const int socket, const std::vector<Client> clients)
{
	for (std::vector<Client>::size_type i = 0; i < clients.size(); ++i)
	{
		if (clients.at(i).getSocket() == socket)
			return (clients.at(i).getNick());
	}
	return ("*");
}
