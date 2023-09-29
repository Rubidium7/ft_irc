/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:38:29 by nlonka            #+#    #+#             */
/*   Updated: 2023/09/29 17:35:39 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_HPP
# define DEFINES_HPP

# include <vector>
//# include <set>

# define MAX_AMOUNT_CLIENTS  5 //1023
# define MAX_AMOUNT_CHANNELS 5 //?
# define MSG_SIZE 512
# define EOM "\n"

typedef enum e_registration
{
	NO_NICK,
	NO_USER,
	NO_PASS,
	REGISTERED
}	t_registration;

typedef enum e_error
{
	NO_ERROR,
	WRONG_ARGS,
	NON_DIGIT_PORT,
	OUT_OF_RANGE_PORT,
	PASSWD_TOO_LONG,
	PASSWD_NON_CHAR,
	TOO_MANY_CLIENTS,
	SERV_SOCKET_FAILURE,
	SERV_BIND_FAILURE,
	SERV_LISTEN_FAILURE,
	SERV_SELECT_FAILURE,
	SERV_ACCEPT_FAILURE
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
	ERR_UNKNOWNERROR = 400,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_TOOMANYTARGETS = 407,
	ERR_NOSUCHSERVICE = 408,
	ERR_NOORIGIN = 409,
	ERR_INVALIDCAPCMD = 410,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_BADCHANNELKEY = 475
}	t_code;

typedef enum e_command
{
	NOT_COMMAND,
	CAP,
	JOIN,
	MODE,
	WHO,
	WHOIS,
	NICK,
	USER,
	PASS,
	PART,
	PRIVMSG,
	PING,
	TOPIC,
	KICK,
	QUIT
}	t_command;

typedef struct s_commands
{
	std::string first_part;
	t_command	command;
}	t_commands;

typedef struct s_message
{
	std::string	msg;
	t_code		code;
}	t_message;

typedef struct s_channel_mode
{
	std::string 		name_of_channel;	// Name of channel
	std::vector<int>		channel_members;	// Who are on channel
	int					invite_only;		// Invite only channel
	int					topic;				// Restrict topic to only ops
	std::string			key;				// channel key (password)
	std::vector<int>		ops_list;			// Who has the ops
	int					limit_users;		// limit of users
}	t_channel_mode;

#endif
