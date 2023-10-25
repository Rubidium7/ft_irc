
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

void	User::userCommand(int socket, Client &client, std::vector<std::string> args, bool debug)
{
	if (!client.getUserName().empty())
	{
		Server::sendAnswer(socket, client.getNick(), ERR_ALREADYREGISTERED, ":Unauthorized command (already registered)", debug);
		return ;
	}
	client.setUserName(args.at(1));
	client.setHostName(args.at(3));
	client.setRealName(_parseRealName(args));
	return ;
}
