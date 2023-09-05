
#include "Client.hpp"

Client::Client() : _socket(0), _nickName("")
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