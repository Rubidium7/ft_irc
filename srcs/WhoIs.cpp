
#include "WhoIs.hpp"
#include "ToolFunctions.hpp"
#include "irc.hpp"

void	WhoIs::whoIsCommand(int socket, Client &client,
		std::string &nick, t_server_mode &serverSettings)
{
	int target_socket = ToolFunctions::findSocketForClientFromName(nick, serverSettings.clients);
	if (!target_socket)
	{
		Server::sendAnswer(socket, client.getNick(), ERR_NOSUCHNICK, nick + " :No such nick", serverSettings.debug);
		return ;
	}
	int i;
	for (i = 0; i < MAX_AMOUNT_CLIENTS; i++)
	{
		if (serverSettings.clients[i].getSocket() == socket)
			break ;
	}
	if (i == MAX_AMOUNT_CLIENTS)
	{
		Server::sendAnswer(socket, client.getNick(), ERR_NOSUCHNICK, nick + " :No such nick", serverSettings.debug);
		return ;
	}
	std::string	msg;

	msg = serverSettings.clients[i].getNick() + " ";
	msg += serverSettings.clients[i].getUserName() + " ";
	msg += serverSettings.clients[i].getHostName() + " * :";
	msg += serverSettings.clients[i].getRealName();
	Server::sendAnswer(socket, client.getNick(), RPL_WHOISUSER, msg, serverSettings.debug);
	Server::sendAnswer(socket, client.getNick(), RPL_ENDOFWHOIS, ":End of WHOIS list.", serverSettings.debug);
}
