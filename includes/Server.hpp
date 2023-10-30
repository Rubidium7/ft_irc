
#ifndef SERVER_HPP
# define SERVER_HPP

# include "defines.hpp"
# include "Parser.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include <string>
# include <vector>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>

typedef struct s_server_mode
{
	std::string					password;
	t_error_code				failure;
	bool						debug;
	int							serverSocket;
	struct sockaddr_in			socketSettings;
	Client						clients[MAX_AMOUNT_CLIENTS];
	std::vector<Channel>		channels;
	fd_set						activeSockets;
	fd_set						readySockets;
	int							maxSocket;
	char						buffer[MSG_SIZE];
	std::vector<std::string>	clientBuffers;
	int							isGollumAwake;
}	t_server_mode;

class Server
{
	public:
		Server(int port, std::string password, bool debug);
		~Server();
		t_error_code	checkFailure();
		void			setReadySockets();
		void			monitorSockets();
		int				getMaxSocket();
		bool			isInSet(int index);
		int				getServerSocket();
		void			newClient();
		static void		sendToOneClient(int socket, std::string msg, bool debug);
		void			clientExit(int socket, t_server_mode &_serverSettings, const std::string &msg);
		void			receiveMessage(int socket);
		static void		sendAnswer(int socket, std::string nick, t_code code, std::string msg, bool debug);

	private:
		Server();
		Server(const Server &src);
		Server	&operator=(const Server &rhs);

		void			_addToClientBuffer(int socket);
		void			_sendMessageFromStruct(int socket, t_message message);
		Client			&_matchClient(int socket);
		int				_findSmallestFreeClientIndex() const;
		bool			_notRegisteredIssue(int socket, Client &client, t_command command, std::vector<std::string> args);
		void			_handleCommands(int socket);
		t_command		_returnFirstPartOfCommand(std::string command) const;
		void			_handleNewRegistration(int socket);
		void			_messageOfTheDay(int socket, std::string &nick);
		void			_newUserMessage(int socket, Client &client);
		void			_handlePing(int socket);
		void			_handleQuit(int socket, std::vector<std::string> args);
		void			_handleJoinColon(int socket);

		t_server_mode		_serverSettings;
		static std::string	_hostName;
};

#endif
