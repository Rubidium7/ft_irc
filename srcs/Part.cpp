/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 17:09:32 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/06 21:06:31 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ToolFunctions.hpp"
#include "../includes/Part.hpp"

void Part::partcmd(int socket, std::string full_command, t_server_mode	&_serverSettings)
{
	std::vector<std::string> commandParts;
	std::vector<std::string> tempChannels;
	ToolFunctions::_split_command_in_parts(full_command, commandParts);

	std::cout << "partcmd entered" << std::endl;

	switch(commandParts.size())
	{
		case 0:
			// Should not come to here
			break;

		case 1:
			// Should not come to here
			break;

		case 2:
			ToolFunctions::_parse_into_parts(commandParts, 1, tempChannels);
			for (std::vector<std::string>::size_type i = 0; i < tempChannels.size(); ++i)
			{
				if (!Server::doesChannelExist(tempChannels.at(i), _serverSettings.channels))
					Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_NOSUCHCHANNEL, ":No such channel");
				for (std::vector<Channel>::size_type k = 0; k < _serverSettings.channels.size(); ++k)
				{
					if (tempChannels.at(i) == _serverSettings.channels.at(k).getChannelName())
					{
						if (_serverSettings.channels.at(k).isOnChannel(socket))
						{
							std::stringstream ss;
							ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients) << "!" << "localhost";
							ss << " PART" << " " << _serverSettings.channels.at(k).getChannelName();
							ss << " :" << std::endl;
							Server::sendToOneClient(socket, ss.str());
							_serverSettings.channels.at(k).partFromChannel(socket);
							ss.str("");
							if (_serverSettings.channels.at(k).howManyMembersOnChannel() == 0)
								_serverSettings.channels.erase(_serverSettings.channels.begin() + k--); // k-- because all channels move one step back
							break ;
						}
						else
						{
							Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_NOTONCHANNEL, ":Not on that channel\n");
							break ;
						}
					}
				}
			}
			break ;

		default:
			std::cout << "Part should not come in here." << std::endl;
			break ;
	}
}

void Part::partFromAllChannels(int socket, t_server_mode &_serverSettings)
{
	for (std::vector<std::string>::size_type i = 0; i < _serverSettings.channels.size(); ++i)
	{
		if (_serverSettings.channels.at(i).isOnChannel(socket))
		{
			std::stringstream ss;
			ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients);
			ss << "!" << "localhost" << " PART " << _serverSettings.channels.at(i).getChannelName() << " :0" << std::endl;
			Server::sendToOneClient(socket, ss.str());
			ss.clear();
			_serverSettings.channels.at(i).partFromChannel(socket);
			if (_serverSettings.channels.at(i).howManyMembersOnChannel() == 0)
				_serverSettings.channels.erase(_serverSettings.channels.begin() + i--); // i-- because all channels move one step back
		}
	}
}

std::string Part::_return_last_part_of_string(int begin, std::string full_command)
{
	std::string temp;
	std::string last_part;
	std::istringstream ss(full_command);

	for (int i = 0; i < begin; ++i)
	{
		ss >> temp;
	}

	if (!ss.fail())
	{
		ss >> std::ws; // Skip leading whitespace
		std::getline(ss, last_part);
		return (last_part);
    }else
		return ("Error: no message even there should be");
}
