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

Server::Server(int port, std::string password) : _hostName("localhost"), _password(password), _failure(NO_ERROR)
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
	_message.msg = "";
	_message.code = EMPTY;
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

bool	Server::isInSet(int socket)
{
	return (FD_ISSET(socket, &_readySockets));
}

int		Server::getServerSocket(void)
{
	return (_serverSocket);
}

void	Server::_assignServerMessage(t_code code, std::string msg)
{
	_message.msg = msg;
	_message.code = code;
}

void	Server::_sendMessageFromStruct(int socket, t_message message)
{
	std::cerr << message.msg << std::endl; //debug
	_sendAnswer(socket, _matchClient(socket).getNick(), message.code, message.msg);
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

void	Server::_sendAnswer(int socket, std::string nick, t_code code, std::string msg)
{
	std::stringstream		message;
	const char				*buffer;
	std::string::size_type	size;

	message << ":" << _hostName << " ";
	if (code < 100)
		message << "0";
	if (code < 10)
		message << "0";
	message << code << " " << nick << " " << msg << "\r\n";
	buffer = message.str().c_str();
	size = message.str().size();
	std::cerr << buffer; //debug
	send(socket, buffer, size, 0);
}

void	Server::sendToOneClient(int socket, std::string msg)
{
	std::stringstream		message;
	const char				*buffer;
	std::string::size_type	size;

	message << msg;
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
		_sendAnswer(new_client, "*", RPL_BOUNCE, ":Server is full");
		close(new_client);
		FD_CLR(new_client, &_activeSockets);
		return ;
	}
	FD_SET(new_client, &_activeSockets);
	if (new_client > _maxSocket)
		_maxSocket = new_client;
	_clients[_clientIndex].setSocket(new_client);
}

void	Server::clientExit(int socket)
{
	close(socket);
	_clientBuffers.at(socket).clear();
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
		// Apparently command handling happens after this ???

		// Print what client sent
		//std::cout << "Client: " << socket << " " << "Sent: #" << _buffer << "#" << std::endl;

		// Add buffer to clientbuffer
		for(int i = 0; _buffer[i]; ++i)
		{
			_clientBuffers.at(socket).push_back(_buffer[i]);
		}

		while (_clientBuffers.at(socket).find(EOM) != std::string::npos)
		{
		//	std::cout << "client id: " << socket << " buffer contents is:" << std::endl
		//	<< _clientBuffers.at(socket) << std::endl;
			_handleCommands(socket);
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

void	Server::_handleCommands(int socket)
{
	t_command	command = _returnFirstPartOfCommand(_clientBuffers.at(socket));

	int newline_pos = _clientBuffers.at(socket).find(EOM);
    std::string full_command =  _clientBuffers.at(socket).substr(0, newline_pos);
	if (EOM == "\n")
		_clientBuffers.at(socket) = _clientBuffers.at(socket).substr(newline_pos + 1);
	else
		_clientBuffers.at(socket) = _clientBuffers.at(socket).substr(newline_pos + 2);
	std::cerr << full_command << std::endl;

	Parser	parser(full_command);

	switch(command)
	{
		case CAP:
			parser.parseCap();
			if (!parser.getMessageCode())
				_handleCap(socket, command, full_command);
			break;
		case JOIN:
			break;
		case MODE:
			break;
		case WHO:
			break;
		case WHOIS:
			break;
		case NICK:
			break;
		case PART:
			break;
		case PRIVMSG:
			break;
		case PING:
			//parser.parsePing();
			if (!parser.getMessageCode())
				_handlePing(socket, full_command);
			break;
		case TOPIC:
			break;
		case KICK:
			break;
		case QUIT:
			break;
		case NOT_COMMAND:
			break;
		default:
			_assignServerMessage(ERR_UNKNOWNCOMMAND, parser.getCommand() + " :Unknown command");
	}
	if (parser.getMessageCode())
		_sendMessageFromStruct(socket, parser.getMessage());
	if (_message.code)
		_sendMessageFromStruct(socket, _message);
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
	std::string first_part;

	ss >> first_part;
	for (int i = 0; i < 12; i++)
	{
		if (commands[i].first_part == first_part)
			return (commands[i].command);
	}
	return (NOT_COMMAND);
}

void	Server::_handleCap(int socket, t_command command, std::string full_command)
{
	(void)socket;
	(void)command;
	(void)full_command;
}

void	Server::_handlePing(int socket, std::string full_command)
{
	(void)full_command;
	sendToOneClient(socket, ":" + _hostName + " PONG " + _hostName + " :" + _hostName + "\r\n");
}
