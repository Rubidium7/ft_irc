/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:35:34 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/09 18:19:17 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kick.hpp"
#include "ToolFunctions.hpp"

void Kick::kickcmd(int socket, std::string full_command, t_server_mode	&_serverSettings)
{
	std::vector<std::string> commandParts;
	std::vector<std::string> tempChannels;
	std::vector<std::string> tempUsers;
	ToolFunctions::_split_command_in_parts(full_command, commandParts);

	std::cout << "kickcmd entered" << std::endl;

	switch(commandParts.size())
	{
		case 0:
			std::cout << "kickcmd should not come to here" << std::endl;
			break;

		case 1:
			std::cout << "kickcmd should not come to here" << std::endl;
			break;

		case 2:
			std::cout << "kickcmd should not come to here" << std::endl;
			break;
		
		case 3:
			ToolFunctions::_parse_into_parts(commandParts, 1, tempChannels);
			ToolFunctions::_parse_into_parts(commandParts, 2, tempUsers);
			
			if (tempChannels.size() == 1)
			{
				if (!Server::doesChannelExist(tempChannels.at(0), _serverSettings.channels))
				{
					std::stringstream ss;
					ss << tempChannels.at(0);
					ss << " :No such channel" << std::endl;
					Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_NOSUCHCHANNEL, ss.str());
					ss.clear();
					return ;
				}
				for (std::vector<Channel>::size_type i = 0; i < _serverSettings.channels.size(); ++i) // Channel exist
				{
					if (_serverSettings.channels.at(i).getChannelName() == tempChannels.at(0))
					{
						if (!_serverSettings.channels.at(i).hasOps(socket))
						{
							std::stringstream ss;
							ss << tempChannels.at(0);
							ss << " :You're not channel operator" << std::endl;
							Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_CHANOPRIVSNEEDED, ss.str());
							ss.clear();
							return ;
						}
						for (std::vector<std::string>::size_type k = 0; k < tempUsers.size(); ++k)
						{
							if (!_serverSettings.channels.at(i).isOnChannel(_returnClientSocket(tempUsers.at(k), _serverSettings)))
							{
								std::stringstream ss;
								ss << tempChannels.at(0);
								ss << " :User is not on that channel" << std::endl;
								Server::sendAnswer(socket, ToolFunctions::_findNickName(socket, _serverSettings.clients), ERR_USERNOTINCHANNEL, ss.str());
								ss.clear();
								continue ;
							}
							{
								_serverSettings.channels.at(i).partFromChannel(_returnClientSocket(tempUsers.at(k), _serverSettings));
								_serverSettings.channels.at(i).setNewOpIfNoOp();
								std::stringstream ss;
								ss << ":" << ToolFunctions::_findNickName(socket, _serverSettings.clients);
								ss << "!" << "localhost" <<  " KICK" << " :" << _serverSettings.channels.at(i).getChannelName() << std::endl;
								Server::sendToOneClient(socket, ss.str());
								Server::sendToOneClient(_returnClientSocket(tempUsers.at(k), _serverSettings), ss.str());
								ss.clear();
								if (_serverSettings.channels.at(i).howManyMembersOnChannel() == 0)
									_serverSettings.channels.erase(_serverSettings.channels.begin() + i);
							}
						}
						break ;
					}
				}
			}else if (tempChannels.size() > 1)
			{
				
			}
			break ;	
		default:
			std::cout << "kickcmd should not come to here" << std::endl;
			break ;
	}
}

int Kick::_returnClientSocket(std::string nick, t_server_mode &_serverSettings)
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (_serverSettings.clients[i].getNick() == nick)
			return (_serverSettings.clients[i].getSocket());
	}
	return (0);
}
