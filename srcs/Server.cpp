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

#include "irc.hpp"

Server::Server(int port, std::string password) : _password(password), _failure(NO_ERROR)
{
	memset(_serverSettings.sin_zero, 0, sizeof(_serverSettings.sin_zero));
	_serverSettings.sin_family = AF_INET;
	_serverSettings.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	_serverSettings.sin_port = htons(port);

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0)
		_failure = SERV_SOCKET_FAILURE;

	if (!_failure && bind(_serverSocket, (struct sockaddr *)&_serverSettings, sizeof(_serverSettings)) < 0)
		_failure = SERV_BIND_FAILURE;

	if (!_failure && listen(_serverSocket, MAX_AMOUNT_CLIENTS) < 0)
		_failure = SERV_LISTEN_FAILURE;
}

Server::~Server()
{

}

t_error_code	Server::checkFailure()
{
	return (_failure);
}