
#include "irc.hpp"
#include "Pass.hpp"

void	Pass::passCommand(int socket, Client &client, std::string password, t_server_mode &serverSettings)
{
	if (client.registrationStatus() != NO_PASS)
		return ;
	if (password != serverSettings.password)
	{
		Server::sendAnswer(socket, client.getNick(), ERR_PASSWDMISMATCH, ":Incorrect password");
		return ;
	}
	client.setGivenPass(true);
	return ;
}
