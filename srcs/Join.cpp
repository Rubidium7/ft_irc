/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:07:55 by tpoho             #+#    #+#             */
/*   Updated: 2023/09/29 17:15:27 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Join.hpp"
#include "../includes/Channel.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

void Join::joincmd(int id, std::string full_command, std::vector<Channel> &_channels)
{
	std::vector<std::string> command_parts;
	std::vector<std::string> temp_channels;
	std::vector<std::string> temp_keys;
	_split_command_in_parts(full_command, command_parts);

	std::cout << "joincmd entered" << std::endl;

	switch(command_parts.size())
	{
		case 2:
			_parse_into_parts(command_parts, 1, temp_channels);
			for (std::vector<std::string>::size_type i = 0; i < temp_channels.size(); ++i)
			{
				for (std::vector<Channel>::size_type k = 0; k < _channels.size(); ++k)
				{
					if (temp_channels.at(i) == _channels.at(k).getChannelName())
					{
						if (_channels.at(k).isOnChannel(id))
						{
							// Is already on channel what to return?
						}else
						{
							_channels.at(k).addToChannel(id);
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
