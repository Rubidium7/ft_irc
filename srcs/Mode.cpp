

#include "Mode.hpp"
#include "ToolFunctions.hpp"
#include "irc.hpp"

bool	Mode::_channelIssues(std::string nick, int socket, std::string chan_name,
	std::vector<Channel> &channels, std::vector<std::string>::size_type	i)
{
	if (i == channels.size())
	{
		Server::sendAnswer(socket, nick,
			ERR_NOSUCHCHANNEL, chan_name + " :No such channel");
		return (true);
	}
	if (!channels.at(i).isOnChannel(socket))
	{
		Server::sendAnswer(socket, nick,
			ERR_NOTONCHANNEL, chan_name + " :You're not on that channel");
		return (true);
	}
	if (!channels.at(i).hasOps(socket))
	{
		Server::sendAnswer(socket, nick,
			ERR_CHANOPRIVSNEEDED, chan_name + " :You're not channel operator");
		return (true);
	}
	return (false);
}

void	Mode::modeCommand(int socket, Client &client,
	std::vector<std::string> args, t_server_mode &serverSettings)
{
	int	target_socket;

	t_mode		mode = Parser::identifyMode(args.at(2));
	std::vector<std::string>::size_type	i;


	if (args.at(1) == client.getNick())
		return ;
	for (i = 0; i < serverSettings.channels.size(); i++)
	{
		if (serverSettings.channels.at(i).getChannelName() == args.at(1))
			break ;
	}
	if (_channelIssues(client.getNick(), socket, args.at(1), serverSettings.channels, i))
		return ;
	switch (mode)
	{
		case I:
			serverSettings.channels.at(i).setInviteMode(ON);
			return ;
		case I_OFF:
			serverSettings.channels.at(i).setInviteMode(OFF);
			return ;
		case T:
			serverSettings.channels.at(i).setTopicMode(ON);
			return ;
		case T_OFF:
			serverSettings.channels.at(i).setTopicMode(OFF);
			return ;
		case K:
			if (serverSettings.channels.at(i).isThereKey())
				Server::sendAnswer(socket, client.getNick(), ERR_KEYSET, args.at(1) + " :Channel key already set");
			else
				serverSettings.channels.at(i).setKey(args.at(3));
			return ;
		case K_OFF:
			if (!serverSettings.channels.at(i).doesKeyMatch(args.at(3)))
				Server::sendAnswer(socket, client.getNick(), ERR_BADCHANNELKEY, args.at(3) + " :Key doesn't match.");
			else
				serverSettings.channels.at(i).setKey("");
			return ;
		case O:
			target_socket = ToolFunctions::_findSocket(args.at(3), serverSettings.clients);
			if (!target_socket || !serverSettings.channels.at(i).isOnChannel(socket))
				Server::sendAnswer(socket, client.getNick(), ERR_USERNOTINCHANNEL,
					args.at(3) + " " + args.at(1) + " :They aren't on that channel");
			else
				serverSettings.channels.at(i).giveOps(target_socket);
			return ;
		case O_OFF:
			target_socket = ToolFunctions::_findSocket(args.at(3), serverSettings.clients);
			if (!target_socket || !serverSettings.channels.at(i).isOnChannel(socket))
				Server::sendAnswer(socket, client.getNick(), ERR_USERNOTINCHANNEL,
					args.at(3) + " " + args.at(1) + " :They aren't on that channel");
			else
				serverSettings.channels.at(i).removeOps(target_socket);
			return ;
		case L:
			serverSettings.channels.at(i).setUserLimit(atoi(args.at(3).c_str()));
			return ;
		case L_OFF:
			serverSettings.channels.at(i).setUserLimit(MAX_AMOUNT_CLIENTS);
			return ;
		default:
			std::cerr << "mode shouldn't get here" << std::endl;
	}
}
