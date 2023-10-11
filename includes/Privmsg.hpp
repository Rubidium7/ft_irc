/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 21:31:51 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/10 18:24:37 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include "defines.hpp"
//# include "Channel.hpp"
# include "Server.hpp"
# include <string>

class Privmsg
{
	public:

		static void privmsgcmd(int socket, std::string full_command, t_server_mode	&_serverSettings);

	private:

		Privmsg();
		~Privmsg();
		Privmsg(Privmsg &copy_constructor);
		Privmsg &operator=(Privmsg &copy_assignment);
};

#endif
