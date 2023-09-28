/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:46:52 by nlonka            #+#    #+#             */
/*   Updated: 2023/09/26 20:51:07 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include "Channel.hpp"
# include <string>
# include <vector>
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
		void			sendToClients(std::string msg);
		void			sendToOneClient(int id, std::string msg);
		void			clientExit(int id);
		void			receiveMessage(int id);

	private:
		Server();
		Server(const Server &src);
		Server	&operator=(const Server &rhs);

		int				_findSmallestFreeClientIndex() const;
		void			_handleCommands(int id);
		t_command		_returnFirstPartOfCommand(std::string command) const;
		void			_handleCap(int id);
		void			_handleJoin(int id);
		void			_handleMode(int id);
		void			_handleWho(int id);
		void			_handleWhois(int id);
		void			_handleNick(int id);
		void			_handlePart(int id);
		void			_handlePrivmsg(int id);
		void			_handlePing(int id);
		void			_handleTopic(int id);
		void			_handleKick(int id);
		void			_handleQuit(int id);
		void			_handleNotCommand(int id);

		std::string					_password;
		t_error_code				_failure;
		int							_serverSocket;
		struct sockaddr_in			_serverSettings;
		Client						_clients[MAX_AMOUNT_CLIENTS];
		std::vector<Channel>		_channels;
		fd_set						_activeSockets;
		fd_set						_readySockets;
		int							_maxSocket;
		char						_buffer[MSG_SIZE]; //temporary solution
		std::vector<std::string>	_clientBuffers;
};

#endif
