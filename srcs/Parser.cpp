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

bool	Parser::_isChannelKeyFormatCorrect(size_t amountOfChannels)
{
	std::string					key_str;
	std::vector<std::string>	keys;
	size_t						comma;

	key_str = _args.at(2);
	while (1)
	{
		comma = key_str.find(',');
		if (comma == std::string::npos)
		{
			keys.push_back(key_str);
			break ;
		}
		keys.push_back(key_str.substr(0, comma));
		key_str.erase(0, comma + 1);
	}
	for (size_t i = 0; i != keys.size(); i++)
	{
		if (keys.at(i).empty())
		{
			_assignParserMessage(ERR_BADCHANNELKEY, keys.at(i) + " :Improper key format");
			return (false);
		}
		if (i == amountOfChannels)
		{
			_assignParserMessage(ERR_TOOMANYTARGETS, keys.at(i) + " :Too many targets");
			return (false);
		}
	}
	return (true);
}

bool	Parser::_isChannelFormatCorrect(size_t *amountOfChannels)
{
	std::string					channel_str;
	std::vector<std::string>	channels;
	size_t						comma;

	channel_str = _args.at(1);
	while (1)
	{
		comma = channel_str.find(',');
		if (comma == std::string::npos)
		{
			channels.push_back(channel_str);
			break ;
		}
		channels.push_back(channel_str.substr(0, comma));
		channel_str.erase(0, comma + 1);
	}
	for (size_t i = 0; i != channels.size(); i++)
	{
		if (channels.at(i).empty() || channels.at(i).at(0) != '#')
		{
			_assignParserMessage(ERR_NOSUCHCHANNEL, channels.at(i) + " :Improper channel format");
			return (false);
		}
		if (channels.at(i).size() < 2)
		{
			_assignParserMessage(ERR_NOSUCHCHANNEL, channels.at(i) + " :Improper channel format");
			return (false);
		}
	}
	*amountOfChannels = channels.size();
	return (true);
}

void	Parser::parseCap()
{
	if (_args.size() < 2)
	{
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
		return ;
	}
	std::string sub_command = _args.at(1);
	if (sub_command != "LS" && sub_command != "END")
		_assignParserMessage(ERR_INVALIDCAPCMD, _args.at(1) + " :Invalid CAP subcommand");
	if (sub_command == "END")
	{
		if (_args.size() != 2)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(2) + " :Too many targets");
		return ;
	}
	if (_args.size() < 3)
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
	else if (_args.size() > 3)
		_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(3) + " :Too many targets");
	else if (_args.at(2) != "302") //we can choose something else to do here as well
		_assignParserMessage(ERR_INVALIDCAPCMD, _args.at(2) + " :Cap version unsupported");
}

void	Parser::parseJoin()
{
	size_t	amountOfChannels = 0;

	if (_args.size() < 2)
	{
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
		return ;
	}
	if (_args.size() > 3)
	{
		_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(3) + " :Too many targets");
		return ;
	}
	if (_args.at(1).at(0) == ':') //should we handle 0?
	{
		if (_args.size() != 2)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(2) + " :Too many targets");
		else if (_args.at(1).size() != 1)
			_assignParserMessage(ERR_NOSUCHCHANNEL, _args.at(1) + " :No such channel");
		return ;
	}
	if (!_isChannelFormatCorrect(&amountOfChannels))
		return ;
	if (_args.size() > 2)
		_isChannelKeyFormatCorrect(amountOfChannels);
}

void	Parser::parseNick()
{
	if (_args.size() < 2)
	{
		_assignParserMessage(ERR_NONICKNAMEGIVEN, _args.at(0) + " :No nickname given");
		return ;
	}
	if (_args.size() > 2)
		_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(2) + " :Too many targets");

	if (_args.at(1).size() > 12)
		_args.at(1).erase(12, std::string::npos);

	std::string	nick = _args.at(1);

	for (size_t i = 0; i < nick.size(); i++)
	{
		if (!isalnum(nick.at(i)) && nick.at(i) != '-' && nick.at(i) != '_')
			_assignParserMessage(ERR_ERRONEUSNICKNAME, _args.at(1) + " :Nick includes weird characters");
	}
}

void	Parser::parsePing(std::string serverName)
{
	if (_args.size() < 2)
	{
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
		return ;
	}
	if (_args.size() > 2)
	{
		_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(2) + " :Too many targets");
		return ;
	}
	if (_args.at(1) != serverName)
		_assignParserMessage(ERR_NOSUCHSERVER, _args.at(1) + " :Server out of scope");
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
