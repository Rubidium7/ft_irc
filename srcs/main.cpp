
#include "irc.hpp"

int main(int ac, char **av)
{
	bool	debug = (ac == 4 && !strcmp(av[3], "debug"));

	if (ac != 3 && !debug)
		return (print_error(WRONG_ARGS));
	std::string	password(av[2]);
	if (password.size() > 100)
		return (print_error(PASSWD_TOO_LONG));
	if (check_characters(password))
		return (print_error(PASSWD_NON_CHAR));
	int	port = check_port(av[1]);
	if (port < 10)
		return (port);

	Server	server(port, password, debug);

	if (server.checkFailure())
		return (print_error(server.checkFailure()));

	while (42)
	{
		server.setReadySockets();
		server.monitorSockets();
		if (server.checkFailure())
			return (print_error(server.checkFailure()));
		for (int id = 0; id <= server.getMaxSocket(); id++)
		{
			if (server.isInSet(id))
			{
				if (server.getServerSocket() == id)
				{
					server.newClient();
					if (server.checkFailure())
						return (print_error(server.checkFailure()));
				}
				else
					server.receiveMessage(id);
			}
		}
	}
	return (0);
}
