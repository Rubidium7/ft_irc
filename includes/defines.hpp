/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:38:29 by nlonka            #+#    #+#             */
/*   Updated: 2023/09/26 19:50:03 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_HPP
# define DEFINES_HPP

# include <vector>

# define MAX_AMOUNT_CLIENTS  5 //1023
# define MAX_AMOUNT_CHANNELS 5 //?
# define MSG_SIZE 512

typedef enum e_message
{
	CLIENT_ARRIVED = 1,
	CLIENT_LEFT,
	CLIENT_MESSAGE
}	t_message;

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

typedef struct s_channel_mode
{
	int					invite_only;	// Invite only channel
	int					topic; 			// Restrict topic to only ops
	std::string			key; 			// channel key (password)
	std::vector<int>	ops_list;		// Who has the ops 
	int					limit_users;	// limit of users
}	t_channel_mode;

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

#endif
