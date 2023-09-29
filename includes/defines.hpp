/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:38:29 by nlonka            #+#    #+#             */
/*   Updated: 2023/09/29 19:30:05 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_HPP
# define DEFINES_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <vector>
# include <string>
//# include <set> 
# include "Client.hpp" 
# include "Channel.hpp"

# define MAX_AMOUNT_CLIENTS  5 //1023
# define MAX_AMOUNT_CHANNELS 5 //?
# define MSG_SIZE 512
# define EOM "\n"

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
	RPL_WELCOME,
	RPL_YOURHOST,
	RPL_CREATED,
	RPL_MYINFO,
	RPL_BOUNCE,
	RPL_HELLO = 20,
	ERR_UNKNOWNERROR = 400,
	ERR_NOSUCHCHANNEL = 403,
	ERR_NOSUCHSERVICE = 408,
	ERR_INVALIDCAPCMD = 410,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464
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

#endif
