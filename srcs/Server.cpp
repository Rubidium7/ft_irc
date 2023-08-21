/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlonka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:53:54 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/17 14:47:28 by nlonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

Server::Server(int port, std::string password) : _password(password), _failure(NO_ERROR), _clientIndex(0)
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

void	Server::sendToClients(int id, t_message type, std::string msg)
{
	std::stringstream		message;
	const char					*buffer;
	std::string::size_type	size;

	if (type == CLIENT_ARRIVED)
		message << "server: client " << id << " just arrived" << std::endl;
	else if (type == CLIENT_LEFT)
		message << "server: client " << id << " left the server" << std::endl;
	else if (type == CLIENT_MESSAGE)
		message << "client " << id << ": " << msg << std::endl;
	buffer = message.str().c_str();
	size = message.str().size();
	for (int i = 0; i < _clientIndex; i++)
		send(_clientSockets[i], buffer, size, 0);
		//empty stream & str
}

void	Server::newClient(void)
{
	int	new_client;

	if (_clientIndex == MAX_AMOUNT_CLIENTS)
	{
		print_error(TOO_MANY_CLIENTS);
		return ;
	}
	new_client = accept(_serverSocket, NULL, NULL);
	if (new_client < 0)
	{
		_failure = SERV_ACCEPT_FAILURE;
		return ;
	}
	FD_SET(new_client, &_activeSockets);
	if (new_client > _maxSocket)
		_maxSocket = new_client;
	_clientSockets[_clientIndex] = new_client;
	sendToClients(_clientIndex, CLIENT_ARRIVED, NULL);
	_clientIndex++;
}

void	Server::clientExit(int id)
{
	sendToClients(id, CLIENT_LEFT, NULL);
	close(id);
	FD_CLR(id, &_activeSockets);
}

void	Server::receiveMessage(int id)
{
	int	bytes_read = recv(id, _buffer, sizeof(_buffer) - 1, 0);
	if (bytes_read <= 0)
	{
		clientExit(id);
	}
	else
	{
		_buffer[bytes_read] = '\0';
		sendToClients(id, CLIENT_MESSAGE, _buffer);
	}
}