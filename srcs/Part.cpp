/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 17:09:32 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/19 15:51:28 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ToolFunctions.hpp"
#include "../includes/Part.hpp"

void Part::partcmd(int socket, std::string full_command, t_server_mode	&_serverSettings)
{
	std::vector<std::string> command_parts;
	std::vector<std::string> temp_channels;
	ToolFunctions::splitCommandInParts(full_command, command_parts);

	std::cout << "partcmd entered" << std::endl;

	switch(command_parts.size())
	{
		case 0:
			// Should not come to here
			break;

		case 1:
			// Should not come to here
			break;

		case 2:
			ToolFunctions::parseIntoParts(command_parts, 1, temp_channels);
			for (std::vector<std::string>::size_type i = 0; i < temp_channels.size(); ++i)
			{
				if (!ToolFunctions::doesChannelExistWithName(temp_channels.at(i), _serverSettings.channels))
					Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_NOSUCHCHANNEL, ":No such channel");
				for (std::vector<Channel>::size_type k = 0; k < _serverSettings.channels.size(); ++k)
				{
					if (temp_channels.at(i) == _serverSettings.channels.at(k).getChannelName())
					{
						if (_serverSettings.channels.at(k).isOnChannel(socket))
						{
							std::stringstream ss;
							ss << ":" << ToolFunctions::findNickName(socket, _serverSettings.clients) << "!" << "localhost";
							ss << " PART" << " " << _serverSettings.channels.at(k).getChannelName();
							ss << " :" << std::endl;
							Server::sendToOneClient(socket, ss.str());
							_serverSettings.channels.at(k).partFromChannel(socket);
							_serverSettings.channels.at(k).setNewOpIfNoOp();
							ss.str("");
							if (_serverSettings.channels.at(k).howManyMembersOnChannel() == 0)
								_serverSettings.channels.erase(_serverSettings.channels.begin() + k--); // k-- because all channels move one step back
							break ;
						}
						else
						{
							Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_NOTONCHANNEL, ":Not on that channel\n");
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
			ss << ":" << ToolFunctions::findNickName(socket, _serverSettings.clients);
			ss << "!" << "localhost" << " PART " << _serverSettings.channels.at(i).getChannelName() << " :0" << std::endl;
			Server::sendToOneClient(socket, ss.str());
			ss.clear();
			_serverSettings.channels.at(i).partFromChannel(socket);
			_serverSettings.channels.at(i).setNewOpIfNoOp();
			if (_serverSettings.channels.at(i).howManyMembersOnChannel() == 0)
				_serverSettings.channels.erase(_serverSettings.channels.begin() + i--); // i-- because all channels move one step back
		}
	}
}

std::string Part::_returnLastPartOfString(int begin, std::string full_command)
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
