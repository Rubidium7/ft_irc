
#ifndef DEFINES_HPP
# define DEFINES_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <vector>
# include <string>

# define NICKLEN 12
# define TOPICLEN 200
# define KICKLEN 200
# define CHANNELLEN 20
# define PARTLEN 200
# define PRIVMSGLEN 200
# define JOINLEN 200
# define MAX_AMOUNT_CLIENTS  5
# define MAX_AMOUNT_CHANNELS 5
# define MSG_SIZE 512
# define EOM "\n"
# define USER_ID(nick, user) (nick + "!" + user + "@localhost")
# define ON 1
# define OFF 0
# define GOLLUM_PASSWORD "MyPrecious"

typedef enum e_registration
{
	REGISTERED,
	NO_PASS,
	NO_NICK,
	NO_USER
}	t_registration;

typedef enum e_error
{
	NO_ERROR,
	WRONG_ARGS,
	NON_DIGIT_PORT,
	OUT_OF_RANGE_PORT,
	PASSWD_TOO_LONG,
	PASSWD_NON_CHAR,
	SERV_SOCKET_FAILURE,
	SERV_BIND_FAILURE,
	SERV_LISTEN_FAILURE,
	SERV_SELECT_FAILURE,
	SERV_ACCEPT_FAILURE,
	SERV_FCNTL_FAILURE
}	t_error_code;

typedef enum e_code
{
	EMPTY,
	RPL_WELCOME = 1,
	RPL_YOURHOST = 2,
	RPL_CREATED = 3,
	RPL_MYINFO = 4,
	RPL_BOUNCE = 5,
	RPL_HELLO = 20,
	RPL_WHOISUSER = 311,
	RPL_ENDOFWHOIS = 318,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICTIME = 333,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_MOTD = 372,
	RPL_MOTDSTART = 375,
	RPL_ENDOFMOTD = 376,
	ERR_UNKNOWNERROR = 400,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_TOOMANYTARGETS = 407,
	ERR_NOSUCHSERVICE = 408,
	ERR_NOORIGIN = 409,
	ERR_INVALIDCAPCMD = 410,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_FILEERROR = 424,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_KEYSET = 467,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_CHANOPRIVSNEEDED = 482
}	t_code;

typedef enum e_command
{
	NOT_COMMAND,
	CAP,
	JOIN,
	MODE,
	INVITE,
	NICK,
	USER,
	PASS,
	PART,
	PRIVMSG,
	PING,
	TOPIC,
	KICK,
	QUIT,
	WHOIS
}	t_command;

typedef enum e_mode
{
	UNKNOWN_MODE,
	I,
	I_OFF,
	T,
	T_OFF,
	K,
	K_OFF,
	O,
	O_OFF,
	L,
	L_OFF
}	t_mode;

typedef struct s_commands
{
	std::string first_part;
	t_command	command;
}	t_commands;

typedef struct s_modes
{
	std::string string;
	t_mode		mode;
}	t_modes;

typedef struct s_message
{
	std::string	msg;
	t_code		code;
}	t_message;

#endif
