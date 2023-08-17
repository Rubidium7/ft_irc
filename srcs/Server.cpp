/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlonka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 13:53:54 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/17 14:47:28 by nlonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "irc.hpp"

Server::Server()
{


}

Server::Server(int port, std::string password) : _password(password)
{}
