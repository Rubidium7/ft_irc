
#ifndef WHOIS_HPP
# define WHOIS_HPP

# include <iostream>

class Client;
typedef struct s_server_mode t_server_mode;

class WhoIs
{
	public:
		static void	whoIsCommand(int socket, Client &client,
			std::string &nick, t_server_mode &serverSettings);

	private:
		WhoIs();
		WhoIs(const WhoIs &src);
		~WhoIs();

		WhoIs &operator=(const WhoIs &rhs);

};

#endif
