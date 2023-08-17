/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlonka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:35:52 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/17 14:15:02 by nlonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
		return (print_error(WRONG_ARGS));
	
	std::string	password(av[2]);
	if (password.size() > 100)
		return (print_error(PASSWD_TOO_LONG));
	if (check_characters(password))
		return (print_error(PASSWD_NON_CHAR));
	int	port = check_port(av[1]);
	if (port < 10)
		return (port);

	return (0);
}
