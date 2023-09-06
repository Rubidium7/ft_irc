/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlonka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:37:48 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/17 11:41:38 by nlonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <iterator>
# include <stdexcept>
# include <unistd.h>
# include <sys/select.h>
# include "defines.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Parser.hpp"

struct IncorrectCommandException : public std::runtime_error{
	IncorrectCommandException(const std::string &msg) : runtime_error(msg){}
};

struct IncorrectArgumentAmountException : public std::runtime_error{
	IncorrectArgumentAmountException(const std::string &msg) : runtime_error(msg){}
};

struct IncorrectCapException : public std::runtime_error{
	IncorrectCapException(const std::string &msg) : runtime_error(msg){}
};

struct IncorrectChannelException : public std::runtime_error{
	IncorrectChannelException(const std::string &msg) : runtime_error(msg){}
};

//errors.c
int		print_error(t_error_code type);

//setup.c
bool	check_characters(std::string password);
int		check_port(char	*av);

#endif
