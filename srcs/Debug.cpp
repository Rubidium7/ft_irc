/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:19:01 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/05 16:18:19 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Debug.hpp"
#include "../includes/ToolFunctions.hpp"

void Debug::debugcmd(int socket, std::string full_command, t_server_mode &_serverSettings)
{
	std::vector<std::string> command_parts;
	ToolFunctions::_split_command_in_parts(full_command, command_parts);

	std::cout << "debugcmd entered" << std::endl;
	(void) socket;

	switch(command_parts.size())
	{
		case 0:
			std::cout << "debugcmd should not come to here" << std::endl;
			break;

		case 1:
			// Poistetaan evaluaatioon mennessa jata toistaiseksi ohjeeksi
			//std::cout << "debugcmd can be used following way:" << std::endl;
			//std::cout << "DEBUG CHANNELS : This will list all channels" << std::endl;
			//std::cout << "DEBUG CHANNELS #channelname: This will list all information that one single channel has" << std::endl;
			//std::cout << "DEBUG CLIENTS : This will list all users and their nicks" << std::endl;
			//std::cout << "DEBUG CLIENTS socket: This will list all inforamation one single user has" << std::endl;
			break;

		case 2:
			if (command_parts.at(1) == "CHANNELS")
			{
				std::cout << "List of Channels:" << std::endl;
				for (std::vector<Channel>::size_type i = 0; i < _serverSettings.channels.size(); ++i)
					std::cout << "Channel: " << _serverSettings.channels.at(i).getChannelName() << std::endl;
			}else if (command_parts.at(1) == "CLIENTS")
			{
				std::cout << "List of Clients:" << std::endl;
				for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
					std::cout << "Socket: " << _serverSettings.clients[i].getSocket() << " " << "Nick: " << _serverSettings.clients[i].getNick() << std::endl;
			}
			break;
		
		case 3:
			if (command_parts.at(1) == "CHANNELS")
			{
				for (std::vector<Channel>::size_type i = 0; i < _serverSettings.channels.size(); ++i)
				{
					if (_serverSettings.channels.at(i).getChannelName() == command_parts.at(2))
					{
						_serverSettings.channels.at(i).printDebug();
						break ;
					}
				}
			}else if (command_parts.at(1) == "CLIENTS")
			{
				for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
				{
					if (_serverSettings.clients[i].getNick() == command_parts.at(2))
					{
						_serverSettings.clients[i].printDebug();
						break ;
					}
				}
			}
			break;
		
		default:
			std::cout << "debugcmd should not come to here" << std::endl;
			break;
	}
}
