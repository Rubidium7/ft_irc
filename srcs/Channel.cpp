/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:03:51 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/05 18:55:48 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include "../includes/defines.hpp"
#include "../includes/Channel.hpp"

Channel::Channel(const std::string name, int socketDescriptor)
{
	_channelSettings.nameOfChannel = name;
	_channelSettings.channelMembers.push_back(socketDescriptor);
	_channelSettings.i = OFF;
	_channelSettings.t = OFF;
	_channelSettings.topic = "";
	_channelSettings.l = MAX_AMOUNT_CLIENTS;
}

Channel::~Channel(void)
{
	// Empty on purpose
}

Channel::Channel(const Channel &copy_constructor)
{
	*this = copy_constructor;
}

Channel	&Channel::operator=(const Channel &copy_assignment)
{
	if (this == &copy_assignment)
		return (*this);

	_channelSettings.nameOfChannel	=  copy_assignment._channelSettings.nameOfChannel;
	_channelSettings.channelMembers = copy_assignment._channelSettings.channelMembers;
	_channelSettings.i				= copy_assignment._channelSettings.i;
	_channelSettings.invitedClients	= copy_assignment._channelSettings.invitedClients;
	_channelSettings.t				= copy_assignment._channelSettings.t;
	_channelSettings.topic			= copy_assignment._channelSettings.topic;
	_channelSettings.k				= copy_assignment._channelSettings.k;
	_channelSettings.o				= copy_assignment._channelSettings.o;
	_channelSettings.l				= copy_assignment._channelSettings.l;

	return (*this);
}

std::string	Channel::getChannelName(void) const
{
	return (_channelSettings.nameOfChannel);
}

std::string	Channel::getTopic(void) const
{
	return (_channelSettings.topic);
}

void	Channel::setTopic(std::string input)
{
	_channelSettings.topic = input;
}

int	Channel::isInviteOnly() const
{
	return (_channelSettings.i);
}

int	Channel::isTopicMode() const
{
	return (_channelSettings.t);
}

int	Channel::isClientInvited(int socket) const
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.invitedClients.size(); ++i)
	{
		if (_channelSettings.invitedClients.at(i) == socket)
			return (1);
	}
	return (0);
}

void	Channel::addInvitation(int socket)
{
	if (isClientInvited(socket))
		return ;
	_channelSettings.invitedClients.push_back(socket);
}

void	Channel::removeInvitation(int socket)
{
	if (!isClientInvited(socket))
		return ;

	for (std::vector<int>::size_type i = 0; i < _channelSettings.invitedClients.size(); ++i)
	{
		if (_channelSettings.invitedClients.at(i) == socket)
		{
			int temp = _channelSettings.invitedClients.at(i);
			_channelSettings.invitedClients.at(i) = _channelSettings.invitedClients.at(_channelSettings.invitedClients.size() - 1);
			_channelSettings.invitedClients.at(_channelSettings.invitedClients.size() - 1) = temp;
			_channelSettings.invitedClients.pop_back();
		}
	}

}

int	Channel::isOnChannel(int id) const
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.channelMembers.size(); ++i)
	{
		if (_channelSettings.channelMembers.at(i) == id)
			return (1);
	}
	return (0);
}

void	Channel::addToChannel(int id)
{
	_channelSettings.channelMembers.push_back(id);
}

void	Channel::partFromChannel(int socket)
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.channelMembers.size(); ++i)
	{
		if (_channelSettings.channelMembers.at(i) == socket)
		{
			std::swap(_channelSettings.channelMembers.at(i),
				_channelSettings.channelMembers.at(_channelSettings.channelMembers.size() -1));
				_channelSettings.channelMembers.pop_back();
			return ;
		}
	}
	return ;
}

int	Channel::howManyMembersOnChannel() const
{
	return (_channelSettings.channelMembers.size());
}

int	Channel::isThereKey() const
{
	return (_channelSettings.k.size());
}

void	Channel::setKey(std::string new_key)
{
	_channelSettings.k = new_key;
}

int	Channel::doesKeyMatch(const std::string &key) const
{
	if (_channelSettings.k == key)
		return (1);
	return (0);
}

void	Channel::printDebug() const
{
	std::cout << "Channel name: " << _channelSettings.nameOfChannel << std::endl;
	std::cout << "Channel members: " << std::endl;
	for (std::vector<int>::size_type i = 0; i < _channelSettings.channelMembers.size(); ++i)
		std::cout << _channelSettings.channelMembers.at(i) << std::endl;
	std::cout << "Invite only: " << _channelSettings.i << std::endl;
	std::cout << "Invited Clients: " << std::endl;
	for (std::vector<int>::size_type i = 0; i < _channelSettings.invitedClients.size(); ++i)
		std::cout << _channelSettings.invitedClients.at(i) << std::endl;
	std::cout << "t: " << _channelSettings.t << std::endl;
	std::cout << "Topic: " << _channelSettings.topic << std::endl;
	std::cout << "Key: " << _channelSettings.k << std::endl;
	std::cout << "Channel Ops: " << std::endl;
	for (std::vector<int>::size_type i = 0; i < _channelSettings.o.size(); ++i)
		std::cout << _channelSettings.o.at(i) << std::endl;
	std::cout << "Limit how many users: " << _channelSettings.l << std::endl;
}

void	Channel::sendToAllChannelMembers(const std::string msg)
{
	std::stringstream		message;
	const char				*buffer;
	std::string::size_type	size;

	message << msg;
	buffer = message.str().c_str();
	size = message.str().size();
	for (std::vector<int>::size_type i = 0; i < _channelSettings.channelMembers.size(); ++i)
	{
		if (_channelSettings.channelMembers.at(i) != 0)
		{
			send(_channelSettings.channelMembers.at(i), buffer, size, 0);
		}
	}
}

const std::vector<int>& Channel::returnChannelMembers() const
{
	return (_channelSettings.channelMembers);
}

int	Channel::hasOps(int socket) const
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.o.size(); ++i)
	{
		if (_channelSettings.o.at(i) == socket)
			return (1);
	}
	return (0);
}

void	Channel::giveOps(int socket)
{
	if (!hasOps(socket))
		_channelSettings.o.push_back(socket);
}

void	Channel::removeOps(int socket)
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.o.size(); i++)
	{
		if (_channelSettings.o.at(i) == socket)
		{
			_channelSettings.o.erase(_channelSettings.o.begin() + i);
			return ;
		}
	}
}

void	Channel::setInviteMode(int mode)
{
	_channelSettings.i = mode;
}

void	Channel::setTopicMode(int mode)
{
	_channelSettings.t = mode;
}

void	Channel::setUserLimit(int amount)
{
	_channelSettings.l = amount;
}
