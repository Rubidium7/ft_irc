
#include "Join.hpp"
#include "Part.hpp"
#include "Mode.hpp"
#include "Nick.hpp"
#include "User.hpp"
#include "Pass.hpp"
#include "Topic.hpp"
#include "Invite.hpp"
#include "Kick.hpp"
#include "Privmsg.hpp"
#include "WhoIs.hpp"

std::string	Server::_hostName = "localhost";

Server::Server(int port, std::string password, bool debug)
{
	_serverSettings.password = password;
	_serverSettings.failure = NO_ERROR;
	_serverSettings.debug = debug;
	memset(_serverSettings.socketSettings.sin_zero, 0, sizeof(_serverSettings.socketSettings.sin_zero));
	_serverSettings.socketSettings.sin_family = AF_INET;
	_serverSettings.socketSettings.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	_serverSettings.socketSettings.sin_port = htons(port);

	_serverSettings.serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSettings.serverSocket < 0)
		_serverSettings.failure = SERV_SOCKET_FAILURE;

	if (!_serverSettings.failure && bind(_serverSettings.serverSocket, (struct sockaddr *)&_serverSettings.socketSettings, sizeof(_serverSettings.socketSettings)) < 0)
		_serverSettings.failure = SERV_BIND_FAILURE;

	if (!_serverSettings.failure && listen(_serverSettings.serverSocket, MAX_AMOUNT_CLIENTS) < 0)
		_serverSettings.failure = SERV_LISTEN_FAILURE;

	if (!_serverSettings.failure && fcntl(_serverSettings.serverSocket, F_SETFL, O_NONBLOCK))
		_serverSettings.failure = SERV_FCNTL_FAILURE;

	FD_ZERO(&_serverSettings.activeSockets);
	FD_SET(_serverSettings.serverSocket, &_serverSettings.activeSockets);
	_serverSettings.maxSocket = _serverSettings.serverSocket; // In the beginning there are no clients so max socket number is servers own socket
	_serverSettings.clientBuffers.reserve(MAX_AMOUNT_CLIENTS + 4); //0,1,2 plus server is 3 clients start from 4
	for (std::vector<std::string>::size_type i = 0; i < _serverSettings.clientBuffers.capacity(); i++)
		_serverSettings.clientBuffers.push_back("");
	_serverSettings.isGollumAwake = 0;
}

Server::~Server()
{
	// Empty on purpose
}

t_error_code
Server::checkFailure(void)
{
	return (_serverSettings.failure);
}

void
Server::setReadySockets(void)
{
	_serverSettings.readySockets = _serverSettings.activeSockets;
}

void
Server::monitorSockets(void)
{
	if (select(_serverSettings.maxSocket + 1, &_serverSettings.readySockets, NULL, NULL, NULL) < 0)
		_serverSettings.failure = SERV_SELECT_FAILURE;
}

int
Server::getMaxSocket()
{
	return (_serverSettings.maxSocket);
}

bool
Server::isInSet(int socket)
{
	return (FD_ISSET(socket, &_serverSettings.readySockets));
}

int
Server::getServerSocket(void)
{
	return (_serverSettings.serverSocket);
}

void
Server::_sendMessageFromStruct(int socket, t_message message)
{
	if (_serverSettings.debug)
		std::cout << message.msg << std::endl; //debug
	sendAnswer(socket, _matchClient(socket).getNick(), message.code, message.msg, _serverSettings.debug);
}

Client	&
Server::_matchClient(int socket)
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; i++)
	{
		if (_serverSettings.clients[i].getSocket() == socket)
		{
			return (_serverSettings.clients[i]);
		}
	}
	return (_serverSettings.clients[0]);
}

void
Server::sendAnswer(int socket, std::string nick, t_code code, std::string msg, bool debug)
{
	std::stringstream		message;
	std::string				tempMessage;
	const char				*buffer;
	std::string::size_type	size;

	message << ":" << _hostName << " ";
	if (code < 100)
		message << "0";
	if (code < 10)
		message << "0";
	message << code << " " << nick << " " << msg << "\r\n";
	tempMessage = message.str();
	message.str("");
	buffer = tempMessage.c_str();
	size = tempMessage.size();
	if (debug)
		std::cout << buffer; //debug
	send(socket, buffer, size, 0);
	buffer = NULL;
}

void
Server::sendToOneClient(int socket, std::string msg, bool debug)
{
	std::stringstream		message;
	std::string				tempMessage;
	const char				*buffer;
	std::string::size_type	size;

	message << msg;
	tempMessage = message.str();
	message.str("");
	buffer = tempMessage.c_str();
	size = tempMessage.size();
	if (debug)
		std::cout << buffer; //debug
	send(socket, buffer, size, 0);
}

void
Server::newClient(void)
{
	int	new_client;
	int _clientIndex = _findSmallestFreeClientIndex();

	new_client = accept(_serverSettings.serverSocket, NULL, NULL);
	if (new_client < 0)
	{
		_serverSettings.failure = SERV_ACCEPT_FAILURE;
		return ;
	}
	if (fcntl(new_client, F_SETFL, O_NONBLOCK))
	{
		_serverSettings.failure = SERV_FCNTL_FAILURE;
		close(new_client);
		return ;
	}
	if (_clientIndex >= MAX_AMOUNT_CLIENTS)
	{
		sendAnswer(new_client, "*", RPL_BOUNCE, ":Server is full", _serverSettings.debug);
		close(new_client);
		return ;
	}
	FD_SET(new_client, &_serverSettings.activeSockets);
	if (new_client > _serverSettings.maxSocket)
		_serverSettings.maxSocket = new_client;
	_serverSettings.clients[_clientIndex].setSocket(new_client);
}

void
Server::clientExit(int socket, t_server_mode &_serverSettings, const std::string &msg)
{
	for (size_t i = 0; i != _serverSettings.channels.size(); i++)
	{
		if (_serverSettings.channels.at(i).isOnChannel(socket))
			_serverSettings.channels.at(i).sendToAllChannelMembers(":" + USER_ID(_matchClient(socket).getNick(),
				_matchClient(socket).getUserName()) + " QUIT" + msg + "\r\n", _serverSettings.debug);
	}
	close(socket);
	_serverSettings.clientBuffers.at(socket).clear();
	FD_CLR(socket, &_serverSettings.activeSockets);
	Part::partFromAllChannels(socket, _serverSettings);
	if (socket == _serverSettings.isGollumAwake)
		_serverSettings.isGollumAwake = 0;
	_matchClient(socket).clearInfo();
}

void
Server::_addToClientBuffer(int socket)
{
	std::string	store;
	std::string	new_content(_serverSettings.buffer);

	store = _serverSettings.clientBuffers.at(socket);
	size_t	size = store.size() + new_content.size();
	if (size > MSG_SIZE)
	{
		if (new_content.size() >= MSG_SIZE)
		{
			new_content.erase(0, new_content.size() - MSG_SIZE);
			_serverSettings.clientBuffers.at(socket).clear();
			_serverSettings.clientBuffers.at(socket) = new_content;
		}
		else
		{
			_serverSettings.clientBuffers.at(socket).erase(0, store.size() + new_content.size() - MSG_SIZE);
			_serverSettings.clientBuffers.at(socket).append(new_content);
		}
	}
	else
	{
		_serverSettings.clientBuffers.at(socket).append(new_content);
	}
}

void
Server::receiveMessage(int socket)
{
	int	bytes_read = recv(socket, _serverSettings.buffer, MSG_SIZE, 0);
	if (bytes_read <= 0)
	{
		clientExit(socket, _serverSettings, " :");
	}
	else
	{
		_serverSettings.buffer[bytes_read] = '\0';
		// Add buffer to clientbuffer
		try
		{
			_addToClientBuffer(socket);
		}
		catch(const std::exception& e)
		{
			// On purpose do nothing
		}

		while (_serverSettings.clientBuffers.at(socket).find(EOM) != std::string::npos)
		{
			_handleCommands(socket);
		}

	}
}

int
Server::_findSmallestFreeClientIndex(void) const
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (_serverSettings.clients[i].getSocket() == 0)
		{
			return (i);
		}
	}
	return (MAX_AMOUNT_CLIENTS);
}

bool
Server::_notRegisteredIssue(int socket, Client &client, t_command command, std::vector<std::string> args)
{
	if (command == NOT_COMMAND)
	{
		sendAnswer(socket, client.getNick(), ERR_UNKNOWNCOMMAND, args.at(0) + " :Unknown command", _serverSettings.debug);
		return (true);
	}
	if (command == CAP || (command == JOIN && args.size() == 2 && args.at(1) == ":"))
		return (false);
	if (command != PASS && !client.hasGivenPass())
	{
		sendAnswer(socket, client.getNick(), ERR_PASSWDMISMATCH, ":You need to give a password", _serverSettings.debug);
		clientExit(socket, _serverSettings, " :");
		return (true);
	}
	if (command != NICK && command != USER && command != PASS && command != QUIT)
	{
		sendAnswer(socket, client.getNick(), ERR_NOTREGISTERED, ":You have not registered", _serverSettings.debug);
		return (true);
	}
	return (false);
}

void
Server::_handleCommands(int socket)
{
	t_command	command = _returnFirstPartOfCommand(_serverSettings.clientBuffers.at(socket));
	int			beginning_status;

	int newline_pos = _serverSettings.clientBuffers.at(socket).find(EOM);
	std::string full_command = _serverSettings.clientBuffers.at(socket).substr(0, newline_pos);
	if (EOM == "\n")
		_serverSettings.clientBuffers.at(socket) = _serverSettings.clientBuffers.at(socket).substr(newline_pos + 1);
	else
		_serverSettings.clientBuffers.at(socket) = _serverSettings.clientBuffers.at(socket).substr(newline_pos + 2);

	if (_serverSettings.debug)
		std::cout << full_command << std::endl; //debug
	beginning_status = _matchClient(socket).registrationStatus();
	Parser	parser(full_command);
	if (!parser.getArgs().size())
		return ;
	if (beginning_status && _notRegisteredIssue(socket, _matchClient(socket), command, parser.getArgs()))
		return ;
	switch(command)
	{
		case CAP:
			parser.parseCap();
			break ;
		case JOIN:
			parser.parseJoin();
			if (parser.getMessageCode())
				break ;
			if (parser.getArgs().at(1) == ":")
				_handleJoinColon(socket);
			else
				Join::joinCommand(socket, full_command, _serverSettings);
			break ;
		case MODE:
			parser.parseMode(_matchClient(socket).getNick());
			if (!parser.getMessageCode())
				Mode::modeCommand(socket, _matchClient(socket), parser.getArgs(), _serverSettings);
			break ;
		case INVITE:
			parser.parseInvite();
			if (!parser.getMessageCode())
			 	Invite::inviteCommand(socket, _matchClient(socket), parser.getArgs(), _serverSettings);
			break ;
		case NICK:
			parser.parseNick();
			if (!parser.getMessageCode())
				Nick::nickCommand(socket, _matchClient(socket), parser.getArgs().at(1), _serverSettings);
			break ;
		case USER:
			parser.parseUser();
			if (!parser.getMessageCode())
				User::userCommand(socket, _matchClient(socket), parser.getArgs(), _serverSettings.debug);
			break ;
		case PASS:
			parser.parsePass();
			if (!parser.getMessageCode())
				Pass::passCommand(socket, _matchClient(socket), parser.getArgs().at(1), _serverSettings);
			break ;
		case PART:
			parser.parsePart();
			if (!parser.getMessageCode())
				Part::partCommand(socket, full_command, _serverSettings);
			break ;
		case PRIVMSG:
			parser.parsePrivmsg();
			if (!parser.getMessageCode())
				Privmsg::privmsgCommand(socket, full_command, _serverSettings);
			break;
		case PING:
			parser.parsePing(_hostName);
			if (!parser.getMessageCode())
				_handlePing(socket);
			break ;
		case TOPIC:
			parser.parseTopic();
			if (!parser.getMessageCode())
				Topic::topicCommand(socket, _matchClient(socket), parser.getArgs(), _serverSettings);
			break ;
		case KICK:
			parser.parseKick();
			if (!parser.getMessageCode())
				Kick::kickCommand(socket, full_command, _serverSettings);
			break ;
		case QUIT:
			parser.parseQuit();
			if (!parser.getMessageCode())
				_handleQuit(socket, parser.getArgs());
			break ;
		case WHOIS:
			parser.parseWhoIs();
			if (!parser.getMessageCode())
				WhoIs::whoIsCommand(socket, _matchClient(socket), parser.getArgs().at(1), _serverSettings);
			break ;
		default:
			sendAnswer(socket, _matchClient(socket).getNick(), ERR_UNKNOWNCOMMAND, parser.getCommand() + " :Unknown command", _serverSettings.debug);
			return ;
	}
	if (parser.getMessageCode())
		_sendMessageFromStruct(socket, parser.getMessage()); //parser has assigned it's possible error msg to a struct and here it sends it
	if (_matchClient(socket).registrationStatus() == REGISTERED && beginning_status)
		_newUserMessage(socket, _matchClient(socket));
}

t_command
Server::_returnFirstPartOfCommand(std::string command) const
{
	t_commands commands[14] = {
		{"CAP", CAP},
		{"JOIN", JOIN},
		{"MODE", MODE},
		{"INVITE", INVITE},
		{"NICK", NICK},
		{"USER", USER},
		{"PASS", PASS},
		{"PART", PART},
		{"PRIVMSG", PRIVMSG},
		{"PING", PING},
		{"TOPIC", TOPIC},
		{"KICK", KICK},
		{"QUIT", QUIT},
		{"WHOIS", WHOIS}
	};
	std::stringstream ss(command);
	std::string first_part;

	ss >> first_part;
	for (size_t i = 0; i != first_part.size(); i++)
		first_part.at(i) = toupper(first_part.at(i));
	for (int i = 0; i < 14; i++)
	{
		if (commands[i].first_part == first_part)
			return (commands[i].command);
	}
	return (NOT_COMMAND);
}

void
Server::_handleJoinColon(int socket)
{
	sendAnswer(socket, _matchClient(socket).getNick(), RPL_HELLO, ":Please wait while we process your connection.", _serverSettings.debug);
	if (_matchClient(socket).registrationStatus() != REGISTERED)
		sendAnswer(socket, _matchClient(socket).getNick(), ERR_NOTREGISTERED, ":You have not registered", _serverSettings.debug);
}

void
Server::_handleQuit(int socket, std::vector<std::string> args)
{
	std::string	msg;

	for (size_t i = 1; i != args.size(); i++)
	{
		msg += " ";
		msg += args.at(i);
	}
	clientExit(socket, _serverSettings, msg);
}

void
Server::_handlePing(int socket)
{
	sendToOneClient(socket, ":" + _hostName + " PONG " + _hostName + " :" + _hostName + "\r\n", _serverSettings.debug);
}

void
Server::_messageOfTheDay(	int socket,
							std::string &nick)
{
	std::string	msg;

	msg = ":- " + _hostName;
	msg += " Message of the Day -";
	sendAnswer(socket, nick, RPL_MOTDSTART, msg, _serverSettings.debug);
	msg.clear();
	msg = ":Hello this is the server woo";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ": █     █░▓█████  ██▓     ▄████▄   ▒█████   ███▄ ▄███▓▓█████    ";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ":▓█░ █ ░█░▓█   ▀ ▓██▒    ▒██▀ ▀█  ▒██▒  ██▒▓██▒▀█▀ ██▒▓█   ▀    ";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ":▒█░ █ ░█ ▒███   ▒██░    ▒▓█    ▄ ▒██░  ██▒▓██    ▓██░▒███      ";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ":░█░ █ ░█ ▒▓█  ▄ ▒██░    ▒▓▓▄ ▄██▒▒██   ██░▒██    ▒██ ▒▓█  ▄    ";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ":░░██▒██▓ ░▒████▒░██████▒▒ ▓███▀ ░░ ████▓▒░▒██▒   ░██▒░▒████▒   ";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ":░ ▓░▒ ▒  ░░ ▒░ ░░ ▒░▓  ░░ ░▒ ▒  ░░ ▒░▒░▒░ ░ ▒░   ░  ░░░ ▒░ ░   ";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ":  ▒ ░ ░   ░ ░  ░░ ░ ▒  ░  ░  ▒     ░ ▒ ▒░ ░  ░      ░ ░ ░  ░   ";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ":  ░   ░     ░     ░ ░   ░        ░ ░ ░ ▒  ░      ░      ░      ";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ":    ░       ░  ░    ░  ░░ ░          ░ ░         ░      ░  ░";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	msg = ":                        ░                                      ";
	sendAnswer(socket, nick, RPL_MOTD, msg, _serverSettings.debug);
	msg.clear();
	sendAnswer(socket, nick, RPL_ENDOFMOTD, ":End of MOTD command.", _serverSettings.debug);
}

void
Server::_newUserMessage(int socket,
						Client &client)
{
	std::string	msg;
	std::string	nick;
	std::string len;
	std::stringstream transform;

	nick = client.getNick();
	msg  = ":Welcome to the server ";
	msg += nick + "!" + client.getUserName() + "@" + client.getHostName();
	sendAnswer(socket, nick, RPL_WELCOME, msg, _serverSettings.debug);
	msg.clear();
	msg = ":Your host is " + _hostName;
	msg += ", running version v0.1";
	sendAnswer(socket, nick, RPL_YOURHOST, msg, _serverSettings.debug);
	msg.clear();
	msg = ":This server was created 17/08/2023 13:53:54";
	sendAnswer(socket, nick, RPL_CREATED, msg, _serverSettings.debug);
	msg.clear();
	msg = _hostName + " v0.1 o iklot";
	sendAnswer(socket, nick, RPL_MYINFO, msg, _serverSettings.debug);
	msg.clear();
	transform << NICKLEN << " ";
	transform << TOPICLEN << " ";
	transform << KICKLEN << " ";
	transform << CHANNELLEN;
	transform >> len;
	msg = "RFC2812 PREFIX=(o)@ CHANTYPES=#+ MODES=1 CHANLIMIT=#+:42 NICKLEN=" + len;
	transform >> len;
	msg += " TOPICLEN=" + len;
	transform >> len;
	msg += " KICKLEN=" + len;
	transform >> len;
	msg += " CHANNELLEN=" + len + " CHANMODES=k,l,i,t";
	msg += " :are supported by this server";
	sendAnswer(socket, nick, RPL_MYINFO, msg, _serverSettings.debug);
	msg.clear();
	_messageOfTheDay(socket, nick);
}
