#ifndef NICK_HPP
# define NICK_HPP

# include <iostream>
class Client;

class Nick
{
	public:
		static void	nickCommand(int socket, Client &client, std::string nick, t_server_mode &serverSettings);

	private:
		Nick();
		Nick(const Nick &src);
		~Nick();

		Nick &operator=(const Nick &rhs);
		static bool	_nickInUse(std::string &nick, Client *clients);
};

#endif
