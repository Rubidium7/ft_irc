#ifndef INVITE_HPP
# define INVITE_HPP

# include <iostream>
# include <vector>

class Client;
typedef struct s_server_mode t_server_mode;

class Invite
{
	public:

		static void	inviteCommand(int socket, Client &client,
			std::vector<std::string> args, t_server_mode &serverSettings);

	private:
		Invite();
		Invite(const Invite &src);
		~Invite();

		Invite &operator=(const Invite &rhs);

};

#endif
