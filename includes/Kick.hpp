/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:34:24 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/12 20:16:44 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
# define KICK_HPP

# include "defines.hpp"
//# include "Channel.hpp"
# include "Server.hpp"
# include <string>

typedef struct s_kickcmd_data
{
	std::vector<std::string>	commandParts;
	std::vector<std::string>	tempChannels;
	std::vector<std::string>	tempUsers;
	int							socket;
	std::string					fullCommand;
}	t_kickcmd_data;

class Kick
{
	public:

		static void kickcmd(int socket, std::string full_command, t_server_mode	&_serverSettings);

	private:

		Kick();
		~Kick();
		Kick(Kick &copy_constructor);
		Kick &operator=(Kick &copy_assignment);

		static int _returnClientSocket(std::string nick, t_server_mode &_serverSettings);
		
		static void _printDoesChannelExistError(t_kickcmd_data &data, std::vector<std::string>::size_type &j, t_server_mode &_serverSettings);
		static void _printYoureNotChannelOperatorError(t_kickcmd_data &data, std::vector<std::string>::size_type &j, t_server_mode &_serverSettings);
		static void _printUserIsNotOnThatChannelError(t_kickcmd_data &data, std::vector<std::string>::size_type &j, t_server_mode &_serverSettings);
		static void _kickUserFromChannel(t_kickcmd_data &data, std::vector<Channel>::size_type &i, std::vector<std::string>::size_type &k, t_server_mode &_serverSettings);
		static int	_goThroughTempUsers(t_kickcmd_data &data, std::vector<std::string>::size_type &j, std::vector<Channel>::size_type &i, t_server_mode &_serverSettings);
};

#endif
