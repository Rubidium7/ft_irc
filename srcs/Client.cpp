/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 17:38:35 by tpoho             #+#    #+#             */
/*   Updated: 2023/09/29 20:23:08 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _socket(0), _nickName("*"), _userName("*"), _realName("*"), _hostName("*"), _registration(NO_NICK), _howManyChannelsJoined(0)
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

int		Client::howManyChannelsJoined() const
{
	return (_howManyChannelsJoined);
}

void		Client::increaseChannelsJoined()
{
	++_howManyChannelsJoined;
}

void	Client::decreaseChannelsJoined()
{
	--_howManyChannelsJoined;
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

int		Client::getRegistrationStatus() const
{
	return (_registration);
}

void	Client::setRegistrationStatus(int status)
{
	_registration = status;
}
