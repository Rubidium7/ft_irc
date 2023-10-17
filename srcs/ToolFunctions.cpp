/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolFunctions.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 17:52:44 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/17 21:38:39 by tpoho            ###   ########.fr       */
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

int	ToolFunctions::findSocketForClientFromName(const std::string nick, const Client *clients)
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (clients[i].getNick() == nick)
			return (clients[i].getSocket());
	}
	return (0);
}

int ToolFunctions::findChannelIndex(const std::string channelName, std::vector<Channel> &channels)
{
	for (std::vector<Channel>::size_type i = 0; i < channels.size(); ++i)
	{
		if (channels.at(i).getChannelName() == channelName)
			return (i);
	}
	return (-1);	
}

int ToolFunctions::_findClientIndexWithSocket(const int socket, const Client clients[])
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (clients[i].getSocket() == socket)
			return (i);
	}
	return (-1);
}

int	ToolFunctions::doesChannelExistWithName(std::string &nameChannel, std::vector<Channel> &channels)
{
	if (findChannelIndex(nameChannel, channels) == -1)
		return (0);
	return (1);
}

void	ToolFunctions::listChannelsToOneSocket(int socket, t_server_mode &_serverSettings, std::stringstream &ss)
{
	ss << "Here be list of channels my master:" << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");

	for (std::vector<Channel>::size_type i = 0; i < _serverSettings.channels.size(); ++i)
	{
		ss << ":Gollum!Mordor PRIVMSG " << ToolFunctions::_findNickName(socket, _serverSettings.clients) << " :";
		ss << "Channel: " << _serverSettings.channels.at(i).getChannelName() << std::endl;
		Server::sendToOneClient(socket, ss.str());
		ss.str("");
	}
}

void	ToolFunctions::listClientsToOneSocket(int socket, Client clients[], std::stringstream &ss)
{
	ss << "Here be list of clients my master:" << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");

	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		ss << "Socket: " << clients[i].getSocket() << " " << "Nick: " << clients[i].getNick() << std::endl;
		Server::sendToOneClient(socket, ss.str());
		ss.str("");
	}
}
