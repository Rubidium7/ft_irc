
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

std::string	Server::_hostName = "localhost";

Server::Server(int port, std::string password)
{
	_serverSettings.password = password;
	_serverSettings.failure = NO_ERROR;
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
	_serverSettings.maxSocket = _serverSettings.serverSocket;
	_serverSettings.clientBuffers.reserve(MAX_AMOUNT_CLIENTS + 4);
	for (std::vector<std::string>::size_type i = 0; i < _serverSettings.clientBuffers.capacity(); i++)
		_serverSettings.clientBuffers.push_back("");
	_serverSettings.message.msg = "";
	_serverSettings.message.code = EMPTY;
	_serverSettings.isGollumAwake = 0;
}

Server::~Server()
{
	// Empty on purpose
}

t_error_code	Server::checkFailure(void)
{
	return (_serverSettings.failure);
}

void	Server::setReadySockets(void)
{
	_serverSettings.readySockets = _serverSettings.activeSockets;
}

void	Server::monitorSockets(void)
{
	if (select(_serverSettings.maxSocket + 1, &_serverSettings.readySockets, NULL, NULL, NULL) < 0)
		_serverSettings.failure = SERV_SELECT_FAILURE;
}

int		Server::getMaxSocket()
{
	return (_serverSettings.maxSocket);
}

bool	Server::isInSet(int socket)
{
	return (FD_ISSET(socket, &_serverSettings.readySockets));
}

int		Server::getServerSocket(void)
{
	return (_serverSettings.serverSocket);
}

void	Server::_clearMessage()
{
	_serverSettings.message.msg = "";
	_serverSettings.message.code = EMPTY;
}

void	Server::_assignServerMessage(t_code code, std::string msg)
{
	_serverSettings.message.msg = msg;
	_serverSettings.message.code = code;
}

void	Server::_sendMessageFromStruct(int socket, t_message message)
{
	std::cerr << message.msg << std::endl; //debug // Onko viela tarpeellinen?
	sendAnswer(socket, _matchClient(socket).getNick(), message.code, message.msg);
}

Client	&Server::_matchClient(int socket)
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; ++i)
	{
		if (_serverSettings.clients[i].getSocket() == socket)
		{
			return (_serverSettings.clients[i]);
		}
	}
	return (_serverSettings.clients[0]);
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
		if (_serverSettings.clients[i].getSocket() != 0)
		{
			// for debug printing
			std::cerr << "i = " << i << " client nro: " << _serverSettings.clients[i].getSocket() << std::endl; // Onko viela tarpeellinen?
			send(_serverSettings.clients[i].getSocket(), buffer, size, 0);
		}
	}
}

void	Server::sendAnswer(int socket, std::string nick, t_code code, std::string msg)
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
	// std::cerr << "sending a message:" << std::endl; //debug
	std::cerr << buffer; //debug // Onko viela tarpeellinen?
	send(socket, buffer, size, 0);
	buffer = NULL;
}

void	Server::sendToOneClient(int socket, std::string msg)
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
	std::cerr << buffer; //debug // Viela tarpeellinen?
	send(socket, buffer, size, 0);
}

void	Server::newClient(void)
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
		return ;
	}
	if (_clientIndex >= MAX_AMOUNT_CLIENTS)
	{
		print_error(TOO_MANY_CLIENTS);
		sendAnswer(new_client, "*", RPL_BOUNCE, ":Server is full");
		close(new_client);
		FD_CLR(new_client, &_serverSettings.activeSockets);
		return ;
	}
	FD_SET(new_client, &_serverSettings.activeSockets);
	if (new_client > _serverSettings.maxSocket)
		_serverSettings.maxSocket = new_client;
	_serverSettings.clients[_clientIndex].setSocket(new_client);
}

void	Server::clientExit(int socket, t_server_mode &_serverSettings, const std::string &msg)
{
	for (size_t i = 0; i != _serverSettings.channels.size(); i++)
	{
		if (_serverSettings.channels.at(i).isOnChannel(socket))
			_serverSettings.channels.at(i).sendToAllChannelMembers(":" + USER_ID(_matchClient(socket).getNick(), _matchClient(socket).getUserName()) + " QUIT" + msg + "\r\n");
	}
	close(socket);
	_serverSettings.clientBuffers.at(socket).clear();
	FD_CLR(socket, &_serverSettings.activeSockets);
	Part::partFromAllChannels(socket, _serverSettings);
	if (socket == _serverSettings.isGollumAwake)
		_serverSettings.isGollumAwake = 0;
	_matchClient(socket).clearInfo();
}

void	Server::receiveMessage(int socket)
{
	int	bytes_read = recv(socket, _serverSettings.buffer, MSG_SIZE, 0);
	if (bytes_read <= 0)
	{
		clientExit(socket, _serverSettings, " :");
	}
	else
	{
		_serverSettings.buffer[bytes_read] = '\0';
		// Print what client sent
		//std::cout << "Client: " << socket << " " << "Sent: #" << _serverSettings.buffer << "#" << std::endl; // Viela tarpeellinen?

		// Add buffer to clientbuffer
		for (int i = 0; _serverSettings.buffer[i]; i++)
		{
			_serverSettings.clientBuffers.at(socket).push_back(_serverSettings.buffer[i]);
		}

		while (_serverSettings.clientBuffers.at(socket).find(EOM) != std::string::npos)
		{
		//	std::cout << "client id: " << socket << " buffer contents is:" << std::endl
		//	<< _clientBuffers.at(socket) << std::endl;
			_handleCommands(socket);
		}

		// sendToClients(_buffer); Needed ??? // Viela tarpeellinen?
	}
}

int	Server::_findSmallestFreeClientIndex(void) const
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

void	Server::_messageOfTheDay(int socket, std::string &nick)
{
	std::string	msg;

	msg = ":- " + _hostName;
	msg += " Message of the Day -";
	sendAnswer(socket, nick, RPL_MOTDSTART, msg);
	msg.clear();
	msg = "Hello this is the server woo";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ": █     █░▓█████  ██▓     ▄████▄   ▒█████   ███▄ ▄███▓▓█████    ";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ":▓█░ █ ░█░▓█   ▀ ▓██▒    ▒██▀ ▀█  ▒██▒  ██▒▓██▒▀█▀ ██▒▓█   ▀    ";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ":▒█░ █ ░█ ▒███   ▒██░    ▒▓█    ▄ ▒██░  ██▒▓██    ▓██░▒███      ";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ":░█░ █ ░█ ▒▓█  ▄ ▒██░    ▒▓▓▄ ▄██▒▒██   ██░▒██    ▒██ ▒▓█  ▄    ";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ":░░██▒██▓ ░▒████▒░██████▒▒ ▓███▀ ░░ ████▓▒░▒██▒   ░██▒░▒████▒   ";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ":░ ▓░▒ ▒  ░░ ▒░ ░░ ▒░▓  ░░ ░▒ ▒  ░░ ▒░▒░▒░ ░ ▒░   ░  ░░░ ▒░ ░   ";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ":  ▒ ░ ░   ░ ░  ░░ ░ ▒  ░  ░  ▒     ░ ▒ ▒░ ░  ░      ░ ░ ░  ░   ";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ":  ░   ░     ░     ░ ░   ░        ░ ░ ░ ▒  ░      ░      ░      ";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ":    ░       ░  ░    ░  ░░ ░          ░ ░         ░      ░  ░";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	msg = ":                        ░                                      ";
	sendAnswer(socket, nick, RPL_MOTD, msg);
	msg.clear();
	sendAnswer(socket, nick, RPL_ENDOFMOTD, ":End of MOTD command.");
}

void	Server::_newUserMessage(int socket, Client &client)
{
	std::string	msg;
	std::string	nick;

	nick = client.getNick();
	msg  = ":Welcome to the server ";
	msg += USER_ID(nick, client.getUserName());
	sendAnswer(socket, nick, RPL_WELCOME, msg);
	msg.clear();
	msg = ":Your host is " + _hostName;
	msg += ", running version v0.1";
	sendAnswer(socket, nick, RPL_YOURHOST, msg);
	msg.clear();
	msg = ":This server was created 17/08/2023 13:53:54"; //just made it up :p // ;)
	sendAnswer(socket, nick, RPL_CREATED, msg);
	msg.clear();
	msg = _hostName + " v0.1 o iklot";
	//<server_name> <version> <usermodes> <chanmodes> // Selittaako tama jotain vai voiko poistaa?
	sendAnswer(socket, nick, RPL_MYINFO, msg);
	msg.clear();
	msg = "RFC2812 PREFIX=(o)@ CHANTYPES=#+ MODES=1 CHANLIMIT=#+:42 NICKLEN=12";
	msg += " TOPICLEN=255 KICKLEN=255 CHANNELLEN=50 CHANMODES=k,l,i,t";
	msg += " :are supported by this server";
	sendAnswer(socket, nick, RPL_MYINFO, msg);
	msg.clear();
	//much more info can be added to 005 msg ^^^	// Viela tarpeellinen?
	_messageOfTheDay(socket, nick);
}

void	Server::_handleCommands(int socket)
{
	t_command	command = _returnFirstPartOfCommand(_serverSettings.clientBuffers.at(socket));
	bool		new_user = false;

	int newline_pos = _serverSettings.clientBuffers.at(socket).find(EOM);
	std::string full_command = _serverSettings.clientBuffers.at(socket).substr(0, newline_pos);
	if (EOM == "\n")
		_serverSettings.clientBuffers.at(socket) = _serverSettings.clientBuffers.at(socket).substr(newline_pos + 1);
	else
		_serverSettings.clientBuffers.at(socket) = _serverSettings.clientBuffers.at(socket).substr(newline_pos + 2);

	std::cout << full_command << std::endl; //debug // Viela tarpeellinen?
	_clearMessage();

	if (_matchClient(socket).registrationStatus() != REGISTERED)
		new_user = true;
	Parser	parser(full_command);
	if (!parser.getArgs().size())
		return ;
	if (new_user && (command != NICK && command != USER && command != QUIT &&
			command != PASS && command != CAP && command != JOIN))
	{
		sendAnswer(socket, _matchClient(socket).getNick(), ERR_NOTREGISTERED, ":You have not registered");
		return ;
	}
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
			else if (_matchClient(socket).registrationStatus() == REGISTERED)
				Join::joinCommand(socket, full_command, _serverSettings);
			else
				_assignServerMessage(ERR_NOTREGISTERED, ":You have not registered");
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
				User::userCommand(socket, _matchClient(socket), parser.getArgs());
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
			if (!parser.getMessageCode()) // Onko tama parseri jo toimiva? Poistin nimittain aivan kaiken virhetarkastelut tasta komennosta
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
			if (!parser.getMessageCode()) // Onko tama parseri jo toimiva? Tama kasky ei enaa tarkasta yhtaan mitaan
				Kick::kickCommand(socket, full_command, _serverSettings);
			break ;
		case QUIT:
			parser.parseQuit();
			if (!parser.getMessageCode())
				_handleQuit(socket, parser.getArgs());
			break ;
		default:
			_assignServerMessage(ERR_UNKNOWNCOMMAND, parser.getCommand() + " :Unknown command");
	}

	if (_serverSettings.message.code)
		_sendMessageFromStruct(socket, _serverSettings.message);
	if (parser.getMessageCode())
		_sendMessageFromStruct(socket, parser.getMessage());
	if (_matchClient(socket).registrationStatus() == REGISTERED && new_user)
		_newUserMessage(socket, _matchClient(socket));
}

t_command		Server::_returnFirstPartOfCommand(std::string command) const
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
		{"DEBUG", DEBUG}
	};
	std::stringstream ss(command);
	std::string first_part;

	ss >> first_part;
	for (int i = 0; i < 14; i++)
	{
		if (commands[i].first_part == first_part)
			return (commands[i].command);
	}
	return (NOT_COMMAND);
}

void	Server::_handleJoinColon(int socket)
{
	sendAnswer(socket, _matchClient(socket).getNick(), RPL_HELLO, ":Please wait while we process your connection.");
	if (_matchClient(socket).registrationStatus() != REGISTERED)
		sendAnswer(socket, _matchClient(socket).getNick(), ERR_NOTREGISTERED, ":You have not registered");
}

void	Server::_handleQuit(int socket, std::vector<std::string> args)
{
	std::string	msg;

	for (size_t i = 1; i != args.size(); i++)
	{
		msg += " ";
		msg += args.at(i);
	}
	clientExit(socket, _serverSettings, msg);
}

void	Server::_handlePing(int socket)
{
	sendToOneClient(socket, ":" + _hostName + " PONG " + _hostName + " :" + _hostName + "\r\n");
}
