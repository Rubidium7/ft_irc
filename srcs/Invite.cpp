

#include "Invite.hpp"
#include "ToolFunctions.hpp"
#include "irc.hpp"

void	Invite::inviteCommand(int socket, Client &client,
	std::vector<std::string> args, t_server_mode &serverSettings)
{
	std::vector<std::string>::size_type	i;
	int target_socket;

	for (i = 0; i < serverSettings.channels.size(); i++)
	{
		if (serverSettings.channels.at(i).getChannelName() == args.at(2))
			break ;
	}
	if (i == serverSettings.channels.size())
		return ;
	if (!serverSettings.channels.at(i).isOnChannel(socket))
	{
		Server::sendAnswer(socket, client.getNick(), ERR_NOTONCHANNEL,
			serverSettings.channels.at(i).getChannelName() + " :You're not on that channel");
		return ;
	}
	if (serverSettings.channels.at(i).isInviteOnly() && !serverSettings.channels.at(i).hasOps(socket))
	{
		Server::sendAnswer(socket, client.getNick(), ERR_CHANOPRIVSNEEDED,
			serverSettings.channels.at(i).getChannelName() + " :You're not channel operator");
		return ;
	}
	target_socket = ToolFunctions::_findSocket(args.at(1), serverSettings.clients);
	if (!target_socket)
	{
		Server::sendAnswer(socket, client.getNick(), ERR_NOSUCHNICK,
			args.at(1) + " :No such nick");
		return ;
	}
	serverSettings.channels.at(i).addInvitation(target_socket);
	Server::sendToOneClient(socket, USER_ID(client.getNick(),
		client.getUserName()) + " INVITE " + args.at(1) + " " + args.at(2) + "\r\n");
}
