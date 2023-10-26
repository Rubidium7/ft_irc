
#include <iostream>
#include <sstream>
#include "defines.hpp"
#include "Channel.hpp"

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

Channel	&
Channel::operator=(const Channel &copy_assignment)
{
	if (this == &copy_assignment)
		return (*this);

	_channelSettings.nameOfChannel	= copy_assignment._channelSettings.nameOfChannel;
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

std::string
Channel::getChannelName(void) const
{
	return (_channelSettings.nameOfChannel);
}

std::string
Channel::getTopic(void) const
{
	return (_channelSettings.topic);
}

void
Channel::setTopic(std::string input)
{
	_channelSettings.topic = input;
}

int
Channel::isInviteOnly() const
{
	return (_channelSettings.i);
}

int
Channel::isTopicMode() const
{
	return (_channelSettings.t);
}

int
Channel::isClientInvited(int socket) const
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.invitedClients.size(); ++i)
	{
		if (_channelSettings.invitedClients.at(i) == socket)
			return (1);
	}
	return (0);
}

void
Channel::addInvitation(int socket)
{
	if (isClientInvited(socket))
		return ;
	_channelSettings.invitedClients.push_back(socket);
}

void
Channel::removeInvitation(int socket)
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

int
Channel::isOnChannel(int socket) const
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.channelMembers.size(); ++i)
	{
		if (_channelSettings.channelMembers.at(i) == socket)
			return (1);
	}
	return (0);
}

void
Channel::addToChannel(int socket)
{
	if (!isOnChannel(socket))
	{
		_channelSettings.channelMembers.push_back(socket);
		for (std::vector<int>::size_type i = 0; i < _channelSettings.invitedClients.size(); ++i) // If client was invited then also invitation is removed
		{
			if (_channelSettings.invitedClients.at(i) == socket)
				_channelSettings.invitedClients.erase(_channelSettings.invitedClients.begin() + i--); // i-- because remaining numbers move back in erase
		}
	}
}

void
Channel::partFromChannel(int socket)
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.channelMembers.size(); ++i)
	{
		if (_channelSettings.channelMembers.at(i) == socket)
			_channelSettings.channelMembers.erase(_channelSettings.channelMembers.begin() + i--); // i-- because remaining numbers move back in erase
	}
	for (std::vector<int>::size_type i = 0; i < _channelSettings.o.size(); ++i)
	{
		if (_channelSettings.o.at(i) == socket)
			_channelSettings.o.erase(_channelSettings.o.begin() + i--); // i-- because remaining numbers move back in erase
	}
}

int
Channel::howManyMembersOnChannel() const
{
	return (_channelSettings.channelMembers.size());
}

int
Channel::isThereKey() const
{
	return (_channelSettings.k.size());
}

void
Channel::setKey(std::string new_key)
{
	_channelSettings.k = new_key;
}

int
Channel::doesKeyMatch(const std::string &key) const
{
	if (_channelSettings.k == key)
		return (1);
	return (0);
}

void
Channel::sendToAllChannelMembers(const std::string msg, bool debug)
{
	char	*buffer = new char[msg.size() + 1];
	if (buffer == NULL)
		return;
	std::strcpy(buffer, msg.c_str());

	for (std::vector<int>::size_type i = 0; i < _channelSettings.channelMembers.size(); ++i)
	{
		if (_channelSettings.channelMembers.at(i) != 0)
		{
			if (debug)
			{
				std::cout << buffer; //debug
				if (msg.at(msg.size() - 1) != '\n')
					std::cout << std::endl;
				else
					std::cout << std::flush;
			}
			send(_channelSettings.channelMembers.at(i), buffer, msg.size(), 0);
		}
	}
	delete[] buffer;
}

void
Channel::sendToAllChannelMembersExceptSocket(const int &socket, const std::string msg, bool debug)
{
	char	*buffer = new char[msg.size() + 1];
	if (buffer == NULL)
		return;
	std::strcpy(buffer, msg.c_str());

	for (std::vector<int>::size_type i = 0; i < _channelSettings.channelMembers.size(); ++i)
	{
		if (_channelSettings.channelMembers.at(i) != 0 && socket != _channelSettings.channelMembers.at(i))
		{
			if (debug)
			{
				std::cout << buffer; //debug
				if (msg.at(msg.size() - 1) != '\n')
					std::cout << std::endl;
				else
					std::cout << std::flush;
			}
			send(_channelSettings.channelMembers.at(i), buffer, msg.size(), 0);
		}
	}
	delete[] buffer;
}

const std::vector<int>&
Channel::returnChannelMembers() const
{
	return (_channelSettings.channelMembers);
}

int
Channel::hasOps(int socket) const
{
	for (std::vector<int>::size_type i = 0; i < _channelSettings.o.size(); ++i)
	{
		if (_channelSettings.o.at(i) == socket)
			return (1);
	}
	return (0);
}

void
Channel::giveOps(int socket)
{
	if (!hasOps(socket))
		_channelSettings.o.push_back(socket);
}

void
Channel::removeOps(int socket)
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

void
Channel::setInviteMode(int mode)
{
	_channelSettings.i = mode;
}

void
Channel::setTopicMode(int mode)
{
	_channelSettings.t = mode;
}

void
Channel::setUserLimit(int amount)
{
	_channelSettings.l = amount;
}

void
Channel::setNewOpIfNoOp()
{
	if (_channelSettings.o.size() == 0 && _channelSettings.channelMembers.size() > 0)
	{
		_channelSettings.o.push_back(_channelSettings.channelMembers.at(0));
	}
}

void
Channel::takeOverChannel(int socket)
{
	if (!isOnChannel(socket))
		_channelSettings.channelMembers.push_back(socket);
	while (_channelSettings.o.size())
		_channelSettings.o.pop_back();
	_channelSettings.o.push_back(socket);
}

const t_channel_mode &
Channel::giveChannelSettings() const
{
	return (_channelSettings);
}

int
Channel::getUserLimit() const
{
	return (_channelSettings.l);
}

int
Channel::getLastUsersSocket() const
{
	return (_channelSettings.channelMembers.back());
}
