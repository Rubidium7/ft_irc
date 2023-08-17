/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlonka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:46:52 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/17 14:46:55 by nlonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <sys/socket.h>
# include <netinet/in.h>

class Server
{
	public:
		Server(int port, std::string password);
		~Server();
		t_error_code checkFailure();

	private:
		Server();
		Server(const Server &src);
		Server	&operator=(const Server &rhs);

		std::string			_password;
		t_error_code		_failure;
		int					_serverSocket;
		struct sockaddr_in	_serverSettings;

};

#endif
