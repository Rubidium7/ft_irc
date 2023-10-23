/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:07:55 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/23 16:36:42 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"
#include "Part.hpp"
#include "ToolFunctions.hpp"

void Join::joinCommand(int socket, std::string full_command, t_server_mode	&_serverSettings)
{
	std::vector<std::string> command_parts; // full_command is split into parts here
	std::vector<std::string> temp_channels; // command_parts.at(1) is split into here
	std::vector<std::string> temp_keys;		// command_parts.at(2) is split into here
	ToolFunctions::splitCommandInParts(full_command, command_parts);

	if (command_parts.size() == 2)
	{
		if (command_parts.at(1) == ":")	// For compatibility with irssi, ":" Is not in a Standard
			return ;
		if (command_parts.at(1) == "0") // Part from all channels
		{
			Part::partFromAllChannels(socket, _serverSettings);
			return ;
		}
	}

	ToolFunctions::parseIntoParts(command_parts, 1, temp_channels);
	if (command_parts.size() == 3)
		ToolFunctions::parseIntoParts(command_parts, 2, temp_keys);
	for (std::vector<std::string>::size_type i = 0; i < temp_channels.size(); ++i) // Go through channels candidates
	{
		if (!ToolFunctions::doesChannelExistWithName(temp_channels.at(i), _serverSettings.channels)) // Channel does not exist so create a new one
		{
			_serverSettings.channels.push_back(Channel(temp_channels.at(i), socket));
			_serverSettings.channels.at(_serverSettings.channels.size() - 1).giveOps(socket);
			std::stringstream ss;
			ss << ":" << ToolFunctions::findNickName(socket, _serverSettings.clients);
			ss << "!" << "localhost" <<  " JOIN " << temp_channels.at(i);
			std::string::size_type position = full_command.find(":");
			if (position == std::string::npos)
				ss << " :" << std::endl;
			else
			{
				ss << " ";
				ss << full_command.substr(position) << std::endl;
			}
			_serverSettings.channels.at(_serverSettings.channels.size() - 1).sendToAllChannelMembers(ss.str());
			ss.str("");
			
			ss << ":localhost " << RPL_TOPIC << " " << ToolFunctions::findNickName(socket, _serverSettings.clients);
			ss << " " << temp_channels.at(i) << " :";
			ss << _serverSettings.channels.at(_serverSettings.channels.size() - 1).getTopic() << std::endl;
			Server::sendToOneClient(socket, ss.str());
			ss.str("");
			continue ;
		}
		for (std::vector<Channel>::size_type k = 0; k < _serverSettings.channels.size(); ++k) // Find existing channels
		{
			if (temp_channels.at(i) == _serverSettings.channels.at(k).getChannelName())
			{
				if (_serverSettings.channels.at(k).isOnChannel(socket)) // Client is already on channel
				{
					// On purpose do nothing if already on a channel
					// Different servers react differently
					// Some do nothing some part and rejoin user
					// There is no standard way to handle this
					break;
				}else if (_serverSettings.channels.at(k).isInviteOnly() && !_serverSettings.channels.at(k).isClientInvited(socket)) // Invite only channel
				{
					std::stringstream ss;
					ss << _serverSettings.channels.at(k).getChannelName();
					ss << " :Cannot join channel (+i)" << std::endl;
					Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_INVITEONLYCHAN, ss.str());
					ss.clear();
					break;
				} else
				{
					if (_serverSettings.channels.at(k).isThereKey()) // Does Channel have key set (password)
					{
						if (i < temp_keys.size()) // Client provides key
						{
							if (_serverSettings.channels.at(k).doesKeyMatch(temp_keys.at(i))) // Key matches
							{
								_serverSettings.channels.at(k).addToChannel(socket);
								std::stringstream ss;
								ss << ":" << ToolFunctions::findNickName(socket, _serverSettings.clients);
								ss << "!" << "localhost" <<  " JOIN " << _serverSettings.channels.at(k).getChannelName() << std::endl;
								std::string::size_type position = full_command.find(":");
								if (position == std::string::npos)
									ss << " :" << std::endl;
								else
								{
									ss << " ";
									ss << full_command.substr(position) << std::endl;
								}
								_serverSettings.channels.at(_serverSettings.channels.size() - 1).sendToAllChannelMembers(ss.str());
								//Server::sendToOneClient(socket, ss.str());
								ss.str("");
								break ;
							}else // Key does not match
							{
								std::stringstream ss;
								ss << _serverSettings.channels.at(k).getChannelName();
								ss << " :Cannot join channel (+k)" << std::endl;
								Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_BADCHANNELKEY, ss.str());
								ss.clear();
								break ;
							}
						}else //Client does not provide key
						{
							std::stringstream ss;
							ss << _serverSettings.channels.at(k).getChannelName();
							ss << " :Cannot join channel (+k)" << std::endl;
							Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_BADCHANNELKEY, ss.str());
							ss.clear();
							break ;
						}
					}else // Channel does not have a key
					{
						_serverSettings.channels.at(k).addToChannel(socket);
						std::stringstream ss;
						ss << ":" << ToolFunctions::findNickName(socket, _serverSettings.clients);
						ss << "!" << "localhost" <<  " JOIN " << _serverSettings.channels.at(k).getChannelName();
						std::string::size_type position = full_command.find(":");
						if (position == std::string::npos)
							ss << " :" << std::endl;
						else
						{
							ss << " ";
							ss << full_command.substr(position) << std::endl;
						}
						_serverSettings.channels.at(_serverSettings.channels.size() - 1).sendToAllChannelMembers(ss.str());
						ss.str("");
						
						ss << ":localhost " << RPL_TOPIC << " " << ToolFunctions::findNickName(socket, _serverSettings.clients);
						ss << " " << _serverSettings.channels.at(k).getChannelName() << " :";
						ss << _serverSettings.channels.at(k).getTopic() << std::endl;
						Server::sendToOneClient(socket, ss.str());
						ss.str("");
						break ;
					}
				}
			}
		}
	}
}

