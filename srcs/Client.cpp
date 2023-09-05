
#include "irc.hpp"

Client::Client() : _socket(0), _id(-1), _nickName("")
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

void	Client::setId(int id)
{
	_id = id;
}

int	Client::getId() const
{
	return (_id);
}

void	Client::runHandShake()
{
	int	bytes_read = 1;
	std::string		message;
	std::ofstream	outfile("client_messages");

	if (!outfile.is_open())
	{
		std::cerr << "client outfile didn't open" << std::endl;
	}
	// while (bytes_read)
	// {
		bytes_read = recv(_socket, _buffer, MSG_SIZE, 0);
		_buffer[bytes_read] = '\0';
		message = _buffer;
		std::cout << message << std::endl;
		outfile << message << std::endl;
		//save to client buffer
	//}

}