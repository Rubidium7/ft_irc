/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:53:54 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/28 20:18:52 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

Server::Server(int port, std::string password) : _password(password), _failure(NO_ERROR), _neededCmd("empty")
{
	memset(_serverSettings.sin_zero, 0, sizeof(_serverSettings.sin_zero));
	_serverSettings.sin_family = AF_INET;
	_serverSettings.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	_serverSettings.sin_port = htons(port);

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0)
		_failure = SERV_SOCKET_FAILURE;

	if (!_failure && bind(_serverSocket, (struct sockaddr *)&_serverSettings, sizeof(_serverSettings)) < 0)
		_failure = SERV_BIND_FAILURE;

	if (!_failure && listen(_serverSocket, MAX_AMOUNT_CLIENTS) < 0)
		_failure = SERV_LISTEN_FAILURE;

	FD_ZERO(&_activeSockets);
	FD_SET(_serverSocket, &_activeSockets);
	_maxSocket = _serverSocket;
}

Server::~Server()
{

}

t_error_code	Server::checkFailure(void)
{
	return (_failure);
}

void	Server::setReadySockets(void)
{
	_readySockets = _activeSockets;
}

void	Server::monitorSockets(void)
{
	if (select(_maxSocket + 1, &_readySockets, NULL, NULL, NULL) < 0)
		_failure = SERV_SELECT_FAILURE;
}

int		Server::getMaxSocket()
{
	return (_maxSocket);
}

bool	Server::isInSet(int id)
{
	return (FD_ISSET(id, &_readySockets));
}

int		Server::getServerSocket(void)
{
	return (_serverSocket);
}

void	Server::_runCommand(std::string command, std::vector<std::string> args, int socket)
{
	if (command == "CAP")
	{
		_neededCmd = "JOIN";
		return ;
	}
	if (command == "JOIN")
	{
		if (_neededCmd == "JOIN" && args.at(1).at(0) == ':')
		{
			sendToOneClient(socket, _matchClient(socket).getNick(), 20, ":Please wait while we process your connection.");
			if (_matchClient(socket).getNick() == "*")
			{
				sendToOneClient(socket, _matchClient(socket).getNick(), 451, ":You have not registered");
				_neededCmd = "NICK";
			}
		}
	}
	_neededCmd = "empty";
}

void	Server::sendToClients(int code, std::string msg)
{
	std::stringstream		message;
	const char				*buffer;
	std::string::size_type	size;

	for (int i = 0; i < MAX_AMOUNT_CLIENTS; i++)
	{
		if (_clients[i].getSocket() != 0)
		{
			message << ":localhost " << code << " " << _clients[i].getNick() << " :" << msg << "\r\n";
			buffer = message.str().c_str();
			size = message.str().size();
			send(_clients[i].getSocket(), buffer, size, 0);
		}
	}
}

void	Server::sendToOneClient(int socket, std::string nick, int code, std::string msg)
{
	std::stringstream		message;
	const char				*buffer;
	std::string::size_type	size;

	message << ":localhost ";
	if (code < 100)
		message << "0";
	if (code < 10)
		message << "0";
	message << code << " " << nick << " " << msg << "\r\n";
	buffer = message.str().c_str();
	size = message.str().size();
	send(socket, buffer, size, 0);
}

void	Server::newClient(void)
{
	int	new_client;
	int _clientIndex = _findSmallestFreeClientIndex();

	new_client = accept(_serverSocket, NULL, NULL);
	if (new_client < 0)
	{
		_failure = SERV_ACCEPT_FAILURE;
		return ;
	}
	if (_clientIndex >= MAX_AMOUNT_CLIENTS)
	{
		print_error(TOO_MANY_CLIENTS);
		sendToOneClient(new_client, "*", 777, "Server is full");
		close(new_client);
		FD_CLR(new_client, &_activeSockets);
		return ;
	}
	FD_SET(new_client, &_activeSockets);
	if (new_client > _maxSocket)
		_maxSocket = new_client;
	_clients[_clientIndex].setSocket(new_client);
	_clients[_clientIndex].setId(_clientIndex);
}

void	Server::clientExit(int socket)
{
	close(socket);
	FD_CLR(socket, &_activeSockets);
	_matchClient(socket).setSocket(0);
}

void	Server::receiveMessage(int socket)
{
	int	bytes_read = recv(socket, _buffer, MSG_SIZE, 0);
	if (bytes_read <= 0)
	{
		clientExit(socket);
	}
	else
	{
		_buffer[bytes_read] = '\0';
		_matchClient(socket).addToBuffer(_buffer);
		Parser	parser(_matchClient(socket).getBuffer());
		while (parser.isEndOfMessage())
		{
			_matchClient(socket).emptyBuffer();
			try
			{
				parser.parse();
			}
			catch (const IncorrectCommandException &e)
			{
				sendToOneClient(socket, _matchClient(socket).getNick(), 421, e.what());
				std::cerr << e.what() << std::endl;
				return ;
			}
			catch (const IncorrectArgumentAmountException &e)
			{
				sendToOneClient(socket, _matchClient(socket).getNick(), 461, e.what());
				std::cerr << e.what() << std::endl;
				return ;
			}
			catch (const IncorrectCapException &e)
			{
				sendToOneClient(socket, _matchClient(socket).getNick(), 410, e.what());
				std::cerr << e.what() << std::endl;
				return ;
			}
			catch (const IncorrectChannelException &e)
			{
				sendToOneClient(socket, _matchClient(socket).getNick(), 403, e.what());
				std::cerr << e.what() << std::endl;
				return ;
			}
			catch (const IncorrectCapVersionException &e)
			{
				sendToOneClient(socket, _matchClient(socket).getNick(), 408, e.what());
				std::cerr << e.what() << std::endl;
				return ;
			}
			catch(const std::exception &e)
			{
				std::cerr << e.what() << std::endl;
				return ;
			}
			//try
			//{
				_runCommand(parser.getCommand(), parser.getArgs(), socket);
			//}
			// catch(const std::exception &e)
			// {
			// 	sendToOneClient(socket, e.what());
			// 	std::cerr << e.what() << std::endl;
			// }
			
		}
	}
}

int		Server::_findSmallestFreeClientIndex() const
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (_clients[i].getSocket() == 0)
		{
			return (i);
		}
	}
	return (MAX_AMOUNT_CLIENTS);
}

Client	&Server::_matchClient(int socket)
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (_clients[i].getSocket() == socket)
		{
			return (_clients[i]);
		}
	}
	return (_clients[0]);
}
