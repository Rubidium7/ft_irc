/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 17:38:35 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/19 13:35:28 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
#include "ToolFunctions.hpp"
#include <sstream>

Client::Client()
{
	_clientSettings.socket = 0;
	_clientSettings.nickName = "*";
	_clientSettings.givenPass = false;
}

Client::~Client()
{

}

int	Client::getSocket() const
{
	return (_clientSettings.socket);
}

void	Client::setSocket(int socket)
{
	_clientSettings.socket = socket;
}

void	Client::setNick(std::string nickName)
{
	_clientSettings.nickName = nickName;
}

std::string	Client::getNick() const
{
	return (_clientSettings.nickName);
}

std::string	Client::getUserName() const
{
	return (_clientSettings.userName);
}

void	Client::setUserName(std::string userName)
{
	_clientSettings.userName = userName;
}

std::string	Client::getRealName() const
{
	return (_clientSettings.realName);
}

void	Client::setRealName(std::string realName)
{
	_clientSettings.realName = realName;
}

std::string	Client::getHostName() const
{
	return (_clientSettings.hostName);
}

void	Client::setHostName(std::string hostName)
{
	_clientSettings.hostName = hostName;
}

void	Client::setGivenPass(bool truth)
{
	_clientSettings.givenPass = truth;
}

void	Client::clearInfo()
{
	_clientSettings.socket = 0;
	_clientSettings.nickName = "*";
	_clientSettings.userName = "";
	_clientSettings.realName = "";
	_clientSettings.hostName = "";
	_clientSettings.givenPass = false;
}

int	Client::registrationStatus() const
{
	if (!_clientSettings.givenPass)
		return (NO_PASS);
	if (_clientSettings.nickName == "*")
		return (NO_NICK);
	if (_clientSettings.userName.empty() || _clientSettings.realName.empty() || _clientSettings.hostName.empty())
		return (NO_USER);
	return (REGISTERED);
}

const t_client_mode	&Client::giveClientSettings() const
{
	return (_clientSettings);
}
