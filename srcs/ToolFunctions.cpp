/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolFunctions.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 17:52:44 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/05 16:22:12 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ToolFunctions.hpp"

void ToolFunctions::_split_command_in_parts(const std::string &full_command, std::vector<std::string> &command_parts)
{
	std::stringstream	ss(full_command);
	std::string			part;
	while (ss >> part)
		command_parts.push_back(part);
}

void ToolFunctions::_parse_into_parts(const std::vector<std::string> &command_parts, int part, std::vector<std::string> &temp_strings)
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

std::string	ToolFunctions::_findNickName(const int socket, const Client clients[])
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (clients[i].getSocket() == socket)
			return (clients[i].getNick());
	}
	return ("*");
}

int	ToolFunctions::_findSocket(const std::string nick, const Client *clients)
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (clients[i].getNick() == nick)
			return (clients[i].getSocket());
	}
	return (0);
}
