
#include "irc.hpp"
#include "User.hpp"

std::string	User::_parseRealName(std::vector<std::string> args)
{
	std::string 	real_name;
	unsigned int	args_size;

	args_size = args.size();
	real_name = args.at(4).erase(0, 1);
	for (unsigned int i = 5; i < args_size; i++)
	{
		real_name += " ";
		real_name += args.at(i);
	}
	return (real_name);
}

bool	User::_weirdCharsInRealName(int socket, std::string nick, bool debug, std::vector<std::string> args)
{
	unsigned int	args_size;

	args_size = args.size();
	if (_weirdChars(args.at(4).erase(0, 1)))
	{
		Server::sendAnswer(socket, nick, ERR_ERRONEUSNICKNAME, args.at(4) + " :weird characters included", debug);
		return (true);
	}
	for (unsigned int i = 5; i < args_size; i++)
	{
		if (_weirdChars(args.at(i)))
		{
			Server::sendAnswer(socket, nick, ERR_ERRONEUSNICKNAME, args.at(i) + " :weird characters included", debug);
			return (true);
		}
	}
	return (false);
}

bool	User::_weirdChars(std::string &name)
{
	for (size_t i = 0; i < name.size(); i++)
	{
		if (!isalnum(name.at(i)) && name.at(i) != '-' && name.at(i) != '_')
			return (true);
	}
	return (false);
}

void	User::userCommand(int socket, Client &client, std::vector<std::string> args, bool debug)
{
	if (!client.getUserName().empty())
	{
		Server::sendAnswer(socket, client.getNick(), ERR_ALREADYREGISTERED, ":Unauthorized command (already registered)", debug);
		return ;
	}
	for (int i = 1; i != 4; i++)
	{
		if (i == 2)
			continue ;
		if (_weirdChars(args.at(i)))
		{
			Server::sendAnswer(socket, client.getNick(), ERR_ERRONEUSNICKNAME, args.at(i) + " :weird characters included", debug);
			return ;
		}
	}
	if (_weirdCharsInRealName(socket, client.getNick(), debug, args))
		return ;
	client.setUserName(args.at(1));
	client.setHostName(args.at(3));
	client.setRealName(_parseRealName(args));
	return ;
}
