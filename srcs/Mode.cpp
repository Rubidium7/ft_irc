
#include "Mode.hpp"
#include "Kick.hpp"
#include "ToolFunctions.hpp"
#include "irc.hpp"

void	Mode::_kickOffExcessUsers(int socket, Channel &channel, t_server_mode &serverSettings)
{
	while (channel.howManyMembersOnChannel() > channel.getUserLimit())
	{
		std::string	kick_cmd = "KICK " + channel.getChannelName() + " ";
		kick_cmd += ToolFunctions::findNickName(channel.getLastUsersSocket(), serverSettings.clients);
		kick_cmd += " :User limit exceeded\r\n";
		Kick::kickCommand(socket, kick_cmd, serverSettings);
	}
}

bool	Mode::_channelIssues(std::string nick, int socket, std::string chan_name,
	std::vector<Channel> &channels, std::vector<std::string>::size_type	i, bool debug)
{
	if (i == channels.size())
	{
		Server::sendAnswer(socket, nick,
			ERR_NOSUCHCHANNEL, chan_name + " :No such channel", debug);
		return (true);
	}
	if (!channels.at(i).isOnChannel(socket))
	{
		Server::sendAnswer(socket, nick,
			ERR_NOTONCHANNEL, chan_name + " :You're not on that channel", debug);
		return (true);
	}
	if (!channels.at(i).hasOps(socket))
	{
		Server::sendAnswer(socket, nick,
			ERR_CHANOPRIVSNEEDED, chan_name + " :You're not channel operator", debug);
		return (true);
	}
	return (false);
}

void	Mode::_modeMessage(Channel &channel, std::string user_id, std::string flag, std::string arg, bool debug)
{
	std::string	msg;

	msg = ":" + user_id + " MODE ";
	msg += channel.getChannelName() + " ";
	msg += flag;
	if (!arg.empty())
		msg += " " + arg;
	msg += "\r\n";
	channel.sendToAllChannelMembers(msg, debug);
}

void	Mode::modeCommand(int socket, Client &client,
	std::vector<std::string> args, t_server_mode &serverSettings)
{
	int			target_socket;
	t_mode		mode = Parser::identifyMode(args.at(2));
	std::vector<std::string>::size_type	i;
	std::string	mode_arg;

	if (args.at(1) == client.getNick()) //if a request for a user mode is sent, it is simply ignored since it's out of this project's scope
		return ;
	for (i = 0; i < serverSettings.channels.size(); i++)
	{
		if (serverSettings.channels.at(i).getChannelName() == args.at(1))
			break ;
	}
	if (_channelIssues(client.getNick(), socket, args.at(1), serverSettings.channels, i, serverSettings.debug))
		return ;
	switch (mode)
	{
		case I:
			serverSettings.channels.at(i).setInviteMode(ON);
			break ;
		case I_OFF:
			serverSettings.channels.at(i).setInviteMode(OFF);
			break ;
		case T:
			serverSettings.channels.at(i).setTopicMode(ON);
			break ;
		case T_OFF:
			serverSettings.channels.at(i).setTopicMode(OFF);
			break ;
		case K:
			if (serverSettings.channels.at(i).isThereKey())
				Server::sendAnswer(socket, client.getNick(), ERR_KEYSET, args.at(1) + " :Channel key already set", serverSettings.debug);
			else
				serverSettings.channels.at(i).setKey(args.at(3));
			return ;
		case K_OFF:
			if (!serverSettings.channels.at(i).doesKeyMatch(args.at(3)))
				Server::sendAnswer(socket, client.getNick(), ERR_BADCHANNELKEY, args.at(3) + " :Key doesn't match.", serverSettings.debug);
			else
				serverSettings.channels.at(i).setKey("");
			return ;
		case O:
			target_socket = ToolFunctions::findSocketForClientFromName(args.at(3), serverSettings.clients);
			if (!target_socket || !serverSettings.channels.at(i).isOnChannel(socket))
				Server::sendAnswer(socket, client.getNick(), ERR_USERNOTINCHANNEL,
					args.at(3) + " " + args.at(1) + " :They aren't on that channel", serverSettings.debug);
			else
				serverSettings.channels.at(i).giveOps(target_socket);
			return ;
		case O_OFF:
			target_socket = ToolFunctions::findSocketForClientFromName(args.at(3), serverSettings.clients);
			if (!target_socket || !serverSettings.channels.at(i).isOnChannel(socket))
				Server::sendAnswer(socket, client.getNick(), ERR_USERNOTINCHANNEL,
					args.at(3) + " " + args.at(1) + " :They aren't on that channel", serverSettings.debug);
			else
				serverSettings.channels.at(i).removeOps(target_socket);
			return ;
		case L:
			serverSettings.channels.at(i).setUserLimit(atoi(args.at(3).c_str())); //parser already checks that the argument is a simple number, so you can trust atoi here
			mode_arg = args.at(3);
			break ;
		case L_OFF:
			serverSettings.channels.at(i).setUserLimit(MAX_AMOUNT_CLIENTS);
			break ;
		default:
			return ;
	}
	_modeMessage(serverSettings.channels.at(i), USER_ID(client.getNick(), client.getUserName()),
		args.at(2), mode_arg, serverSettings.debug);
	_kickOffExcessUsers(socket, serverSettings.channels.at(i), serverSettings);
}
