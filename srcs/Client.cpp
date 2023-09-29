/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 17:38:35 by tpoho             #+#    #+#             */
/*   Updated: 2023/09/29 19:57:14 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(): _socket(0), _nickName("*"), _howManyChannelsJoined(0)
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