/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:46:52 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/28 20:12:19 by tpoho            ###   ########.fr       */
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
		t_error_code	checkFailure();
		void			setReadySockets();
		void			monitorSockets();
		int				getMaxSocket();
		bool			isInSet(int index);
		int				getServerSocket();
		void			newClient();
		void			sendToClients(int id, t_message type, std::string msg);
		void			sendToOneClient(int id, t_message type, std::string msg);
		void			clientExit(int id);
		void			receiveMessage(int id);

	private:
		Server();
		Server(const Server &src);
		Server	&operator=(const Server &rhs);

		int				_findSmallestFreeClientIndex() const;

		std::string			_password;
		t_error_code		_failure;
		int					_serverSocket;
		struct sockaddr_in	_serverSettings;
		int					_clientSockets[MAX_AMOUNT_CLIENTS];
		fd_set				_activeSockets;
		fd_set				_readySockets;
		int					_maxSocket;
		char				_buffer[20000]; //temporary solution
};

#endif
