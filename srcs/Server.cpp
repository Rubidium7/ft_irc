/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:53:54 by nlonka            #+#    #+#             */
/*   Updated: 2023/09/28 12:12:19 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "../includes/defines.hpp"

Server::Server(int port, std::string password) : _password(password), _failure(NO_ERROR)
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
	_clientBuffers.reserve(MAX_AMOUNT_CLIENTS + 4);
	for(std::vector<std::string>::size_type i = 0; i < _clientBuffers.capacity(); ++i)
		_clientBuffers.push_back("");
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

void	Server::sendToClients(std::string msg)
{
	std::stringstream		message;
	const char				*buffer;
	std::string::size_type	size;

	message << msg;
	buffer = message.str().c_str();
	size = message.str().size();
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; i++)
	{
		if (_clients[i].getSocket() != 0)
		{
			// for debug printing
			std::cerr << "i = " << i << " client nro: " << _clients[i].getSocket() << std::endl;
			send(_clients[i].getSocket(), buffer, size, 0);
		}
	}
}

void	Server::sendToOneClient(int id, std::string msg)
{
	std::stringstream		message;
	const char				*buffer;
	std::string::size_type	size;

	message << msg;
	buffer = message.str().c_str();
	size = message.str().size();
	send(id, buffer, size, 0);
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
		sendToOneClient(new_client, "Too many clients on a server");
		close(new_client);
		FD_CLR(new_client, &_activeSockets);
		return ;
	}
	FD_SET(new_client, &_activeSockets);
	if (new_client > _maxSocket)
		_maxSocket = new_client;
	_clients[_clientIndex].setSocket(new_client);
}

void	Server::clientExit(int id)
{
	close(id);
	_clientBuffers.at(id).clear();
	FD_CLR(id, &_activeSockets);
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (_clients[i].getSocket() == id)
		{
			_clients[i].setSocket(0);
			break ;
		}
	}
}

void	Server::receiveMessage(int id)
{
	int	bytes_read = recv(id, _buffer, MSG_SIZE, 0);
	if (bytes_read <= 0)
	{
		clientExit(id);
	}
	else
	{
		_buffer[bytes_read] = '\0';
		// Apparently command handling happens after this ???
		
		// Print what client sent
		std::cout << "Client: " << id << " " << "Sent: #" << _buffer << "#" << std::endl;
		
		// Add buffer to clientbuffer
		for(int i = 0; _buffer[i]; ++i)
		{
			_clientBuffers.at(id).push_back(_buffer[i]);
		}
	
		while (_clientBuffers.at(id).find('\n') != std::string::npos)
		{
			std::cout << "client id: " << id << " buffer contents is:\n"
			<< _clientBuffers.at(id) << std::endl;
			_handleCommands(id);
		}
				
		// sendToClients(_buffer); Needed ???
	}
}

int	Server::_findSmallestFreeClientIndex(void) const
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

void	Server::_handleCommands(int id)
{
	t_command	command = _returnFirstPartOfCommand(_clientBuffers.at(id));

	switch(command)
	{
		case CAP:
			_handleCap(id);
			break;
		case JOIN:
			_handleJoin(id);
			break;
		case MODE:
			_handleMode(id);
			break;
		case WHO:
			_handleWho(id);
			break;
		case WHOIS:
			_handleWhois(id);
			break;
		case NICK:
			_handleNick(id);
			break;
		case PART:
			_handlePart(id);
			break;
		case PRIVMSG:
			_handlePrivmsg(id);
			break;
		case PING:
			_handlePing(id);
			break;
		case TOPIC:
			_handleTopic(id);
			break;
		case KICK:
			_handleKick(id);
			break;
		case QUIT:
			_handleQuit(id);
			break;
		case NOT_COMMAND:
			_handleNotCommand(id);
			break;
		default:
			std::cout << "Not a valid command." << std::endl;
	}
}

t_command		Server::_returnFirstPartOfCommand(std::string command) const
{
	t_commands commands[12] = {
        {"CAP", CAP},
        {"JOIN", JOIN},
        {"MODE", MODE},
        {"WHO", WHO},
        {"WHOIS", WHOIS},
		{"NICK", NICK},
		{"PART", PART},
		{"PRIVMSG", PRIVMSG},
		{"PING", PING},
		{"TOPIC", TOPIC},
		{"KICK", KICK},
		{"QUIT", QUIT}
    };
	std::stringstream ss(command);
	std::string firstPart;

	ss >> firstPart;
	for (int i = 0; i < 12; ++i)
	{
		if (commands[i].first_part == firstPart)
			return (commands[i].command);
	}
	return (NOT_COMMAND);
}

void	Server::_handleCap(int id)
{
	std::cout << "Cap" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handleJoin(int id)
{
	std::cout << "Join" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handleMode(int id)
{
	std::cout << "Mode" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handleWho(int id)
{
	std::cout << "Who" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handleWhois(int id)
{
	std::cout << "Whois" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handleNick(int id)
{
	std::cout << "Nick" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handlePart(int id)
{
	std::cout << "Part" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handlePrivmsg(int id)
{
	std::cout << "Privmsg" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handlePing(int id)
{
	std::cout << "Ping" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handleTopic(int id)
{
	std::cout << "Topic" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handleKick(int id)
{
	std::cout << "Kick" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handleQuit(int id)
{
	std::cout << "Quit" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}

void	Server::_handleNotCommand(int id)
{
	std::cout << "Not_command" << std::endl;
	int newLinePos = _clientBuffers.at(id).find('\n');
    std::string full_command =  _clientBuffers.at(id).substr(0, newLinePos);
	_clientBuffers.at(id) = _clientBuffers.at(id).substr(newLinePos + 1);
}
