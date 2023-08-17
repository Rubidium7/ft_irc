/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlonka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:37:14 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/17 14:01:47 by nlonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int	print_error(t_error_code type)
{
	if (type == WRONG_ARGS)
		std::cerr << "[USAGE]: ./ircserv <port> <password>" << std::endl;
	if (type == PASSWD_TOO_LONG)
		std::cerr << "password has to be 100 characters or less" << std::endl;
	if (type == PASSWD_NON_CHAR)
		std::cerr << "password can only include letters or digits" << std::endl;
	if (type == NON_DIGIT_PORT)
		std::cerr << "port has to include only digits" << std::endl;
	if (type == OUT_OF_RANGE_PORT)
		std::cerr << "port has to be between the numbers 1024 and 49151" << std::endl;
	return (type);
}
