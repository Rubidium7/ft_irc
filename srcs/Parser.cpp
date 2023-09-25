#include "irc.hpp"

Parser::Parser(std::string input) : _wholeInput(input), _command("empty")
{
	_commandOptions[0] = "CAP";
	_commandOptions[1] = "KICK";
	_commandOptions[2] = "INVITE";
	_commandOptions[3] = "TOPIC";
	_commandOptions[4] = "MODE";
	_commandOptions[5] = "NICK";
	_commandOptions[6] = "JOIN";
	_commandOptions[7] = "PASS";
	_commandOptions[8] = "PRIVMSG";
	_commandOptions[9] = "empty";
}

Parser::~Parser()
{}

std::string	Parser::getCommand() const
{
	return (_command);
}

std::vector<std::string>	Parser::getArgs() const
{
	return (_args);
}

bool	Parser::isEndOfMessage()
{
	size_t	found;

	found = _wholeInput.find("\r\n"); //doesn't work with nc
	_input = _wholeInput.substr(0, found + 2);
	_wholeInput.erase(0, found + 2);
	if (found != std::string::npos)
		return (true);
	else
		return (false);
}

void	Parser::_emptyCommand()
{
	_args.clear();
	_command = "empty";
}

void	Parser::_parseCap()
{
	if (_args.size() < 2)
		throw IncorrectArgumentAmountException(_command + " :Not enough parameters");
	std::string sub_command = _args.at(1);
	if (sub_command != "LS" && sub_command != "LIST"
		&& sub_command != "REQ" && sub_command != "END")
		throw IncorrectCapException(_command + " :Invalid CAP subcommand");
	if (sub_command != "LS")
		return ;
	if (_args.size() < 3)
		throw IncorrectArgumentAmountException(_command + " :Not enough parameters");
	std::string cap_version = _args.at(2);
	if (cap_version != "302")
		throw IncorrectCapVersionException(_command + " :Unsupported CAP version"); //not exactly correct

}

void	Parser::_parseJoin()
{
	if (_args.size() < 2)
		throw IncorrectArgumentAmountException(_command + " :Not enough parameters");
	if (_args.at(1).at(0) != ':' && _args.at(1).at(0) != '#')
		throw IncorrectChannelException(_args.at(1) + " :No such channel");
}

void	Parser::_saveArguments()
{
	std::stringstream	process(_input);
	std::string			word;

	_args.clear();
	process >> word;
	while (!word.empty())
	{
		_args.push_back(word);
		word.clear();
		process >> word;
	}
	_command = _args.front();
}

void	Parser::parse()
{
	std::string			word;
	int					i;

	_saveArguments();
	for (i = 0; i != 10; i++)
		if (_commandOptions[i] == _command)
			break ;
	switch (i)
	{
		case 0:
			_parseCap();
			break ;
		case 1:

			break ;
		case 2:

			break ;
		case 3:

			break ;
		case 4:

			break ;
		case 5:

			break ;
		case 6:
			_parseJoin();
			break ;
		case 7:

			break ;
		case 8:

			break ;
		case 9:
			break ;
		default :
			_emptyCommand();
			throw IncorrectCommandException(_command + " :Unknown command");
	}
	std::cout << "command is: " << _command << std::endl; //debug
}
