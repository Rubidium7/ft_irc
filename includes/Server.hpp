/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:46:52 by nlonka            #+#    #+#             */
/*   Updated: 2023/09/29 20:02:43 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "defines.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Parser.hpp"
# include <string>
# include <vector>
# include <sys/socket.h>
# include <netinet/in.h>

typedef struct s_server_mode
{
	std::string					hostName;
	std::string					password;
	t_error_code				failure;
	t_message					message;
	int							serverSocket;
	struct sockaddr_in			socketSettings;
	Client						clients[MAX_AMOUNT_CLIENTS];
	std::vector<Channel>		channels;
	fd_set						activeSockets;
	fd_set						readySockets;
	int							maxSocket;
	char						buffer[MSG_SIZE]; //temporary solution
	std::vector<std::string>	clientBuffers;
}	t_server_mode;

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
		void			sendToOneClient(int socket, std::string msg);
		void			clientExit(int socket);
		void			receiveMessage(int socket);
		void			sendAnswer(int socket, std::string nick, t_code code, std::string msg);
	
	private:
		Server();
		Server(const Server &src);
		Server	&operator=(const Server &rhs);

		void			_sendMessageFromStruct(int socket, t_message message);
		Client			&_matchClient(int socket);
		int				_findSmallestFreeClientIndex() const;
		void			_handleCommands(int socket);
		t_command		_returnFirstPartOfCommand(std::string command) const;
		void			_handleCap(int socket, t_command command, std::string full_command);
		// void			_handleMode(int id);
		// void			_handleWho(int id);
		// void			_handleWhois(int id);
		// void			_handleNick(int id);
		// void			_handlePart(int id);
		// void			_handlePrivmsg(int id);
		void			_handlePing(int socket, std::string full_command);
		// void			_handleTopic(int id);
		// void			_handleKick(int id);
		// void			_handleQuit(int id);
		void			_handleNotCommand(int socket);
		
		void			_assignServerMessage(t_code code, std::string msg);

		t_server_mode	_serverSettings;
};

#endif
