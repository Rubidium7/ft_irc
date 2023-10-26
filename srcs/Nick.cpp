
#include "irc.hpp"
#include "Nick.hpp"

bool	Nick::_nickInUse(std::string &nick, Client *clients)
{
	for (int i = 0; i < MAX_AMOUNT_CLIENTS; i++)
	{
		if (clients[i].getSocket() != 0)
		{
			if (clients[i].getNick() == nick)
				return (true);
		}
	}
	return (false);
}

void	Nick::nickCommand(int socket, Client &client, std::string nick, t_server_mode &serverSettings)
{
	if (client.getNick() == nick)
		return ;
	if (nick == "Gollum")
	{
		Server::sendAnswer(socket, client.getNick(), ERR_ERRONEUSNICKNAME, nick + " :Nickname reserved.", serverSettings.debug);
		return ;
	}
	if (_nickInUse(nick, serverSettings.clients))
	{
		Server::sendAnswer(socket, client.getNick(), ERR_NICKNAMEINUSE, nick + " :Nickname is already in use.", serverSettings.debug);
		return ;
	}
	if (!client.getUserName().empty())
		Server::sendToOneClient(socket, ":" + USER_ID(client.getNick(),
			client.getUserName()) + " NICK " + nick + "\r\n", serverSettings.debug);
	client.setNick(nick);
}
