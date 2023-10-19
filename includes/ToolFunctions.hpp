/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolFunctions.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 17:53:10 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/19 13:40:02 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLFUNCTIONS_HPP
# define TOOLFUNCTIONS_HPP

# include "Server.hpp"
# include <string>

class ToolFunctions
{
	public:

		static void 		_split_command_in_parts(const std::string &full_command, std::vector<std::string> &command_parts);
		static void 		_parse_into_parts(const std::vector<std::string> &command_parts, int part, std::vector<std::string> &temp_strings);
		static std::string	_findNickName(const int socket, const Client clients[]);
		static int			findSocketForClientFromName(const std::string nick, const Client *clients);
		static int			findChannelIndex(const std::string channelName, std::vector<Channel> &channels);
		static int			_findClientIndexWithSocket(const int socket, const Client clients[]);
		static int			doesChannelExistWithName(std::string &nameChannel, std::vector<Channel> &channels);
		static void			listChannelsToOneSocket(int socket, t_server_mode &_serverSettings, std::stringstream &ss);
		static void			listClientsToOneSocket(int socket, Client clients[], std::stringstream &ss);
		static void			printClientInformation(int socket, const t_client_mode &_clientSettings, t_server_mode &_serverSettings);
		static void			printChannelInformation(int socket, const t_channel_mode &_channelSettings, t_server_mode &_serverSettings);

	private:

		ToolFunctions();
		~ToolFunctions();
		ToolFunctions(ToolFunctions &copy_constructor);
		ToolFunctions &operator=(ToolFunctions &copy_assignment);
};

#endif
