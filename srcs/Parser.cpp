#include "Parser.hpp"

Parser::Parser(std::string input) : _input(input)
{}

Parser::~Parser()
{}

bool	Parser::isEndOfMessage()
{
	size_t	found;

	found = _input.find("\r\n"); //mightn't work with nc
	if (found != std::string::npos)
		return (true);
	else
		return (false);
}

void	Parser::parse()
{
	std::stringstream	process(_input);
	std::string			word;

	process >> _command;

	//switch () command
}

std::string	Parser::getCommand() const
{
	return (_command);
}