
#include "irc.hpp"

Client::Client() : _socket(0), _id(-1), _nickName("*")
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

void	Client::setNick(std::string nickName)
{
	_nickName = nickName;
}


std::string	Client::getNick() const
{
	return (_nickName);
}

void	Client::addToBuffer(std::string text)
{
	_buffer += text;
	std::cout << _buffer; //debug
}

std::string	Client::getBuffer() const
{
	return (_buffer);
}

void	Client::emptyBuffer()
{
	_buffer.clear();
}

// void	Client::runHandShake()
// {
// 	int	bytes_read;
// 	std::ofstream	outfile("client_messages");

// 	if (!outfile.is_open())
// 	{
// 		std::cerr << "client outfile didn't open" << std::endl;
// 	}
// 	bytes_read = recv(_id, _buffer, MSG_SIZE, 0);
// 	_buffer[bytes_read] = '\0';
// 	std::cout << _buffer << std::endl;
// 	outfile << _buffer << std::endl;
// }
