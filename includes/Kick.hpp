/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:34:24 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/09 17:02:10 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
# define KICK_HPP

# include "defines.hpp"
//# include "Channel.hpp"
# include "Server.hpp"
# include <string>

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
};

#endif