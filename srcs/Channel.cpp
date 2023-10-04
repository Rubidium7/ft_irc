/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:03:51 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/03 20:50:14 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/defines.hpp"
#include "../includes/Channel.hpp"

Channel::Channel(const std::string name, int socketDescriptor)
{
	_channelSettings.channel_members.push_back(socketDescriptor);
	_channelSettings.name_of_channel = name;
	_channelSettings.invite_only = 0;
	_channelSettings.topic = 0;
	_channelSettings.limit_users = MAX_AMOUNT_CLIENTS;
}

Channel::~Channel(void)
{
	// Empty on purpose
}

Channel::Channel(const Channel &copy_constructor)
{
	_channelSettings.name_of_channel = copy_constructor._channelSettings.name_of_channel;
	_channelSettings.channel_members = copy_constructor._channelSettings.channel_members;
	_channelSettings.invite_only = copy_constructor._channelSettings.invite_only;
	_channelSettings.topic = copy_constructor._channelSettings.topic;
	_channelSettings.key = copy_constructor._channelSettings.key;
	_channelSettings.ops = copy_constructor._channelSettings.ops;
	_channelSettings.limit_users = copy_constructor._channelSettings.limit_users;
}

Channel &Channel::operator=(const Channel &copy_assignment)
{
	if (this == &copy_assignment)
		return (*this);

	_channelSettings.name_of_channel = copy_assignment._channelSettings.name_of_channel;
	_channelSettings.channel_members = copy_assignment._channelSettings.channel_members;
	_channelSettings.invite_only = copy_assignment._channelSettings.invite_only;
	_channelSettings.topic = copy_assignment._channelSettings.topic;
	_channelSettings.key = copy_assignment._channelSettings.key;
	_channelSettings.ops = copy_assignment._channelSettings.ops;
	_channelSettings.limit_users = copy_assignment._channelSettings.limit_users;
	return (*this);
}

std::string	Channel::getChannelName(void) const
{
	return (_channelSettings.name_of_channel);
}

int	Channel::isOnChannel(int id) const
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.channel_members.size(); ++i)
	{
		if (_channelSettings.channel_members.at(i) == id)
			return (1);
	}
	return (0);
}

void Channel::addToChannel(int id)
{
	_channelSettings.channel_members.push_back(id);
}

void Channel::partFromChannel(int socket)
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.channel_members.size(); ++i)
	{
		if (_channelSettings.channel_members.at(i) == socket)
		{
			std::swap(_channelSettings.channel_members.at(i),
				_channelSettings.channel_members.at(_channelSettings.channel_members.size() -1));
				_channelSettings.channel_members.pop_back();
			return ;
		}
	}
	return ;
}

int	Channel::howManyMembersOnChannel() const
{
	return (_channelSettings.channel_members.size());
}

int	Channel::isThereKey() const
{
	return (_channelSettings.key.size());		
}

void Channel::setKey(std::string new_key)
{
	_channelSettings.key = new_key;
}

void Channel::printDebug() const
{
	std::cout << "Channelname: " << _channelSettings.name_of_channel << std::endl;
	std::cout << "Channel members: " << std::endl;
	for (std::vector<int>::size_type i = 0; i < _channelSettings.channel_members.size(); ++i)
		std::cout << _channelSettings.channel_members.at(i) << std::endl;
		std::cout << "Invite only: " << _channelSettings.invite_only << std::endl;
	std::cout << "Topic: " << _channelSettings.topic << std::endl;
	std::cout << "Key: " << _channelSettings.key << std::endl;
	std::cout << "Channel Ops: " << std::endl;
	for (std::vector<int>::size_type i = 0; i < _channelSettings.ops.size(); ++i)
		std::cout << _channelSettings.ops.at(i) << std::endl;
	std::cout << "Limit how many users: " << _channelSettings.limit_users << std::endl;
}