
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
		return ; //idk if this should be an error or not
	if (nick == "Gollum")
	{
		Server::sendAnswer(socket, client.getNick(), ERR_ERRONEUSNICKNAME, nick + " :Nickname reserved.");
		return ;
	}
	if (_nickInUse(nick, serverSettings.clients))
	{
		Server::sendAnswer(socket, client.getNick(), ERR_NICKNAMEINUSE, nick + " :Nickname is already in use.");
		return ;
	}
	Server::sendToOneClient(socket, ":" + USER_ID(client.getNick(), client.getUserName()) + " NICK " + nick + "\r\n");
	client.setNick(nick);
}
