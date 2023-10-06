/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 17:38:35 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/05 14:55:57 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

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

void	Client::printDebug() const
{
	std::cout << "Client information:" << std::endl;
	std::cout << "Socket: " << _socket << std::endl;
	std::cout << "Nick: " << _nickName << std::endl;
	std::cout << "Username: " << _userName << std::endl;
	std::cout << "Realname: " << _realName << std::endl;
	std::cout << "Hostname: " << _hostName << std::endl;
	std::cout << "Pass given: " << _givenPass << std::endl;
}
