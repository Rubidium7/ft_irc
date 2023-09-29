#include "irc.hpp"

Parser::Parser(std::string full_command)
{
	_message.msg = "";
	_message.code = EMPTY;
	_saveArguments(full_command);
}

Parser::~Parser()
{}

std::vector<std::string>	Parser::getArgs() const
{
	return (_args);
}

std::string	Parser::getCommand() const
{
	return (_args.at(0));
}

t_code	Parser::getMessageCode() const
{
	return (_message.code);
}

t_message	Parser::getMessage() const
{
	return (_message);
}

void	Parser::_assignParserMessage(t_code code, std::string msg)
{
	_message.msg = msg;
	_message.code = code;
}

void	Parser::parseCap()
{
	if (_args.size() < 2)
	{
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
		return ;
	}
	std::string sub_commmand = _args.at(1);
	if (sub_commmand != "LS" && sub_commmand != "LIST"
		&& sub_commmand != "REQ" && sub_commmand != "END")
		_assignParserMessage(ERR_INVALIDCAPCMD, _args.at(0) + " :Invalid CAP subcommand");
}

void	Parser::parseJoin()
{
	if (_args.size() < 2)
	{
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
		return ;
	}
	if (_args.at(1).at(0) != ':' && _args.at(1).at(0) != '#')
	{
		_assignParserMessage(ERR_NOSUCHCHANNEL, _args.at(1) + " :No such channel");
	}
}

void	Parser::_saveArguments(std::string input)
{
	std::stringstream	process(input);
	std::string			word;

	_args.clear();
	process >> word;
	while (!word.empty())
	{
		_args.push_back(word);
		word.clear();
		process >> word;
	}
}
