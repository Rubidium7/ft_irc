/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlonka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:37:14 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/17 11:41:06 by nlonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int	print_error(t_error_code type)
{
	if (type == WRONG_ARGS)
		std::cerr << "[USAGE]: ./ircserv <port> <password>" << std::endl;
	return (type);
}
