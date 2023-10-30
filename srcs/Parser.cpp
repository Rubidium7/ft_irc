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
	if (_args.empty())
		return ("");
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

bool	Parser::_hasWeirdChars(std::string str)
{
	for (size_t i = 0; i != str.size(); i++)
	{
		if (!isalnum(str.at(i)) && str.at(i) != '_' && str.at(i) != '-')
		{
			return (true);
		}
	}
	return (false);
}

std::vector<std::string>	Parser::_createVector(std::string string, char separator)
{
	std::vector<std::string>	ans;
	size_t						found;

	while (1)
	{
		found = string.find(separator);
		if (found == std::string::npos)
		{
			ans.push_back(string);
			break ;
		}
		ans.push_back(string.substr(0, found));
		string.erase(0, found + 1);
	}
	return (ans);
}

bool	Parser::_isChannelKeyFormatCorrect(size_t amountOfChannels)
{
	std::vector<std::string>	keys;

	keys = _createVector(_args.at(2), ',');
	for (size_t i = 0; i != keys.size(); i++)
	{
		if (keys.at(i).empty() || _hasWeirdChars(keys.at(i)))
		{
			_assignParserMessage(ERR_BADCHANNELKEY, keys.at(i) + " :Improper key format");
			return (false);
		}
		if (i == amountOfChannels && amountOfChannels)
		{
			_assignParserMessage(ERR_TOOMANYTARGETS, keys.at(i) + " :Too many targets");
			return (false);
		}
	}
	return (true);
}

bool	Parser::_isChannelFormatCorrect(size_t *amountOfChannels)
{
	std::vector<std::string>	channels;

	channels = _createVector(_args.at(1), ',');
	for (size_t i = 0; i != channels.size(); i++)
	{
		if (channels.at(i).empty() || channels.at(i).at(0) != '#')
		{
			_assignParserMessage(ERR_NOSUCHCHANNEL, channels.at(i) + " :Improper channel format");
			return (false);
		}
		if (channels.at(i).size() < 2 || _hasWeirdChars(channels.at(i).substr(1, std::string::npos)))
		{
			_assignParserMessage(ERR_NOSUCHCHANNEL, channels.at(i) + " :Improper channel format");
			return (false);
		}
	}
	if (amountOfChannels)
		*amountOfChannels = channels.size();
	return (true);
}

t_mode	Parser::identifyMode(std::string input)
{
	t_modes modes[10] = {
		{"+i", I},
		{"-i", I_OFF},
		{"+t", T},
		{"-t", T_OFF},
		{"+k", K},
		{"-k", K_OFF},
		{"+o", O},
		{"-o", O_OFF},
		{"+l", L},
		{"-l", L_OFF}
	};
	for (int i = 0; i != 10; i++)
	{
		if (input == modes[i].string)
			return (modes[i].mode);
	}
	return (UNKNOWN_MODE);
}

bool	Parser::_onlyNumeric(std::string input)
{
	for (size_t i = 0; i != input.size(); i++)
	{
		if (!isdigit(input.at(i)))
			return (false);
	}
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
		_assignParserMessage(ERR_INVALIDCAPCMD, sub_command + " :Invalid CAP subcommand");
	else if (sub_command == "END")
	{
		if (_args.size() != 2)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(2) + " :Too many targets");
		return ;
	}
	else if (_args.size() < 3)
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
	else if (_args.size() > 3)
		_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(3) + " :Too many targets");
	else if (_args.at(2) != "302")
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
	if (_args.at(1) == ":" || _args.at(1) == "0")
	{
		if (_args.size() != 2)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(2) + " :Too many targets");
		return ;
	}
	if (!_isChannelFormatCorrect(&amountOfChannels))
		return ;
	if (_args.size() > 2)
		_isChannelKeyFormatCorrect(amountOfChannels);
}

void	Parser::parseInvite()
{
	if (_args.size() < 3)
	{
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
		return ;
	}
	if (_args.size() > 3)
	{
		_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(3) + " :Too many targets");
		return ;
	}
	if (_args.at(2).front() != '#' || _args.at(2).size() < 2 || _hasWeirdChars(_args.at(2).substr(1, std::string::npos)))
	{
		_assignParserMessage(ERR_NOSUCHCHANNEL, _args.at(2) + " :Improper channel format");
		return ;
	}
}

void	Parser::parseNick()
{
	if (_args.size() < 2)
	{
		_assignParserMessage(ERR_NONICKNAMEGIVEN, ":No nickname given");
		return ;
	}
	if (_args.size() > 2)
	{
		_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(2) + " :Too many targets");
		return ;
	}
	if (_args.at(1).size() > NICKLEN)
		_args.at(1).erase(NICKLEN, std::string::npos);

	std::string	nick = _args.at(1);

	for (size_t i = 0; i < nick.size(); i++)
	{
		if (!isalnum(nick.at(i)) && nick.at(i) != '-' && nick.at(i) != '_')
		{
			_assignParserMessage(ERR_ERRONEUSNICKNAME, _args.at(1) + " :Erroneous nickname");
			return ;
		}
	}
}

void	Parser::parseUser()
{
	if (_args.size() < 5)
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
	else if (_args.at(4).front() != ':'|| _args.at(4).size() < 2)
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Incorrect format");
}

void	Parser::parsePass()
{
	if (_args.size() < 2)
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
}

void	Parser::parsePart()
{
	if (_args.size() < 2)
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
	else if (!_isChannelFormatCorrect(NULL))
		return ;
	else if (_args.size() != 2 && _args.at(2).front() != ':')
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(1) + " :Improper comment format");
}

void	Parser::parseQuit()
{
	if (_args.size() > 1 && _args.at(1).front() != ':')
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Improper leave message format");
}

void	Parser::parseKick()
{
	if (_args.size() < 3)
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
	else if (_args.size() > 3 && _args.at(3).front() != ':')
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Improper kick message format");
	else if (!_isChannelFormatCorrect(NULL))
		_assignParserMessage(ERR_NOSUCHCHANNEL, _args.at(1) + " :Improper channel format");
	else if (!_isChannelKeyFormatCorrect(0))
		_assignParserMessage(ERR_NOSUCHCHANNEL, _args.at(2) + " :Improper nick format");
}

void	Parser::parseTopic()
{
	if (_args.size() < 2)
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
	else if (_args.at(1).front() != '#' || _args.at(1).size() < 2)
		_assignParserMessage(ERR_NOSUCHCHANNEL, _args.at(1) + " :Improper channel format");
	else if (_args.size() > 2 && _args.at(2).front() != ':')
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Improper topic set format");
}

void	Parser::parseMode(std::string nick)
{
	if (_args.size() < 3)
	{
		_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(0) + " :Not enough parameters");
		return ;
	}
	if (_args.at(1) == nick) //if a request for a user mode is sent, it is simply ignored since it's out of this project's scope
		return ;
	if (_args.at(1).front() != '#' || _args.at(1).size() < 2 || _hasWeirdChars(_args.at(1).substr(1, std::string::npos)))
	{
		_assignParserMessage(ERR_NOSUCHCHANNEL, _args.at(1) + " :Improper channel format");
		return ;
	}
	t_mode	mode = identifyMode(_args.at(2));
	switch (mode)
	{
		case I:
		if (_args.size() > 3)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :No parameters for " + _args.at(2));
			return ;
		case I_OFF:
		if (_args.size() > 3)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :No parameters for " + _args.at(2));
			return ;
		case T:
		if (_args.size() > 3)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :No parameters for " + _args.at(2));
			return ;
		case T_OFF:
		if (_args.size() > 3)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :No parameters for " + _args.at(2));
			return ;
		case K:
		if (_args.size() > 4)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :only one parameter for " + _args.at(2));
		else if (_args.size() != 4)
			_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(2) + " :Not enough parameters");
			return ;
		case K_OFF:
		if (_args.size() > 4)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :only one parameter for " + _args.at(2));
		else if (_args.size() != 4)
			_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(2) + " :Not enough parameters");
			return ;
		case O:
		if (_args.size() > 4)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :only one parameter for " + _args.at(2));
		else if (_args.size() != 4)
			_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(2) + " :Not enough parameters");
			return ;
		case O_OFF:
		if (_args.size() > 4)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :only one parameter for " + _args.at(2));
		else if (_args.size() != 4)
			_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(2) + " :Not enough parameters");
			return ;
		case L:
		if (_args.size() > 4)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :only one parameter for " + _args.at(2));
		else if (_args.size() != 4)
			_assignParserMessage(ERR_NEEDMOREPARAMS, _args.at(2) + " :Not enough parameters");
		else if (!_onlyNumeric(_args.at(3)) || _args.at(3).size() > 6)
			_assignParserMessage(ERR_UNKNOWNMODE, _args.at(3) + " :should only include numbers for mode +l");
			return ;
		case L_OFF:
		if (_args.size() > 3)
			_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(0) + " :No parameters for " + _args.at(2));
			return ;
		default:
			_assignParserMessage(ERR_UNKNOWNMODE, _args.at(2) + " :is unknown mode char to me for " + _args.at(1));
	}
}

void	Parser::parsePrivmsg()
{
	if (_args.size() < 2 || _args.at(1).front() == ':')
		_assignParserMessage(ERR_NORECIPIENT, ":No recipient given " + _args.at(0));
	else if (_args.size() < 3)
		_assignParserMessage(ERR_NOTEXTTOSEND, ":No text to send");
	else if (_args.at(2).front() != ':')
		_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(1) + " :Too many targets");
	else if (_args.at(2).size() < 2)
		_assignParserMessage(ERR_NOTEXTTOSEND, ":No text to send");
}

void	Parser::parseWhoIs()
{
	if (_args.size() < 2)
		_assignParserMessage(ERR_NONICKNAMEGIVEN, ":No nickname given");
	else if (_args.size() > 2)
		_assignParserMessage(ERR_TOOMANYTARGETS, _args.at(2) + " :Too many targets");
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
