/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:03:51 by tpoho             #+#    #+#             */
/*   Updated: 2023/09/26 18:06:16 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel(const std::string name, int socketDescriptor): _nameOfChannel(name)
{
	_channelMembers.push_back(socketDescriptor);
}

Channel::~Channel(void)
{
	// Empty on purpose
}

std::string	Channel::getChannelName(void) const
{
	return (_nameOfChannel);
}
