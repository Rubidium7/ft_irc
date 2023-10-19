/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:35:52 by nlonka            #+#    #+#             */
/*   Updated: 2023/10/19 18:49:09 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

//fcntl(fd, F_SETFL, O_NONBLOCK); (!)

int main(int ac, char **av)
{
	if (ac != 3)
		return (print_error(WRONG_ARGS));
	
	std::string	password(av[2]);
	if (password.size() > 100)
		return (print_error(PASSWD_TOO_LONG));
	if (check_characters(password))
		return (print_error(PASSWD_NON_CHAR));
	int	port = check_port(av[1]);
	if (port < 10)
		return (port);

	Server	server(port, password);

	if (server.checkFailure())
		return (print_error(server.checkFailure()));
	
	while (42)
	{
		server.setReadySockets();
		server.monitorSockets();
		if (server.checkFailure())
			return (print_error(server.checkFailure()));
		for (int id = 0; id <= server.getMaxSocket(); id++)
		{
			if (server.isInSet(id))
			{
				if (server.getServerSocket() == id)
				{
					server.newClient();
					if (server.checkFailure())
						return (print_error(server.checkFailure()));
				}
				else
					server.receiveMessage(id);
			}
		}
	}
	return (0);
}
