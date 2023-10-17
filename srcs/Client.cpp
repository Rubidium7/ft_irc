/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 17:38:35 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/17 20:40:39 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include <sstream>

Client::Client():
	_socket(0), _nickName("*"), _givenPass(false)
{

}

Client::~Client()
{

}

int	Client::getSocket() const
{
	return (_socket);
}

void	Client::setSocket(int socket)
{
	_socket = socket;
}

void	Client::setNick(std::string nickName)
{
	_nickName = nickName;
}

std::string	Client::getNick() const
{
	return (_nickName);
}

std::string	Client::getUserName() const
{
	return (_userName);
}

void	Client::setUserName(std::string userName)
{
	_userName = userName;
}

std::string	Client::getRealName() const
{
	return (_realName);
}

void	Client::setRealName(std::string realName)
{
	_realName = realName;
}

std::string	Client::getHostName() const
{
	return (_hostName);
}

void	Client::setHostName(std::string hostName)
{
	_hostName = hostName;
}

void	Client::setGivenPass(bool truth)
{
	_givenPass = truth;
}

void	Client::clearInfo()
{
	_socket = 0;
	_nickName = "*";
	_userName = "";
	_realName = "";
	_hostName = "";
	_givenPass = false;
}

int	Client::registrationStatus() const
{
	if (!_givenPass)
		return (NO_PASS);
	if (_nickName == "*")
		return (NO_NICK);
	if (_userName.empty() || _realName.empty() || _hostName.empty())
		return (NO_USER);
	return (REGISTERED);
}

void	Client::printClientInformation(int socket) const
{
	std::stringstream ss;
	ss << "Client information:" << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");

	ss << "Socket: " << _socket << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");

	ss << "Nick: " << _nickName << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");

	ss << "Username: " << _userName << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");

	ss << "Realname: " << _realName << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");

	ss << "Hostname: " << _hostName << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");

	ss << "Pass given: " << _givenPass << std::endl;
	Server::sendToOneClient(socket, ss.str());
	ss.str("");
}
