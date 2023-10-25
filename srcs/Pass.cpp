
#include "irc.hpp"
#include "Pass.hpp"

void	Pass::passCommand(int socket, Client &client, std::string password, t_server_mode &serverSettings)
{
	if (client.registrationStatus() == REGISTERED)
	{
		Server::sendAnswer(socket, client.getNick(), ERR_ALREADYREGISTERED, ":Unauthorized command (already registered)", serverSettings.debug);
		return ;
	}
	if (client.hasGivenPass())
		return ;
	if (password != serverSettings.password)
	{
		Server::sendAnswer(socket, client.getNick(), ERR_PASSWDMISMATCH, ":Incorrect password", serverSettings.debug);
		return ;
	}
	client.setGivenPass(true);
	return ;
}
