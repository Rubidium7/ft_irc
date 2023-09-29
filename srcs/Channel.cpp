/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:03:51 by tpoho             #+#    #+#             */
/*   Updated: 2023/09/29 19:49:39 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	Channel::addToChannel(int id)
{
	_channelSettings.channel_members.push_back(id);
}
