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

# include "Client.hpp"
# include "Parser.hpp"
# include <string>
# include <fstream>
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
		void			sendToClients(int code, std::string msg);
		void			sendToOneClient(int socket, std::string nick, int code, std::string msg);
		void			clientExit(int id);
		void			receiveMessage(int id);

	private:
		Server();
		Server(const Server &src);
		Server	&operator=(const Server &rhs);

		int				_findSmallestFreeClientIndex() const;
		std::string		_parseRealName(std::vector<std::string> args);
		bool			_nickInUse(std::string &nick) const;
		Client			&_matchClient(int socket);
		void			_newUserMessage(int socket);
		void			_messageOfTheDay(int socket, std::string &nick);
		void			_runCommand(std::string command, std::vector<std::string> args, int socket);

		std::string			_hostName;
		std::string			_password;
		t_error_code		_failure;
		int					_serverSocket;
		struct sockaddr_in	_serverSettings;
		Client				_clients[MAX_AMOUNT_CLIENTS];
		fd_set				_activeSockets;
		fd_set				_readySockets;
		int					_maxSocket;
		char				_buffer[MSG_SIZE]; //temporary solution
		std::string			_neededCmd;
};

#endif
