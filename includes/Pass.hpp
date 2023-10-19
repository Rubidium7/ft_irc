#ifndef PASS_HPP
# define PASS_HPP

# include <iostream>

class Client;

class Pass
{
	public:
		static void passCommand(int socket, Client &client, std::string password, t_server_mode &serverSettings);

	private:
		Pass();
		Pass(const Pass &src);
		~Pass();

		Pass &operator=(const Pass &rhs);
};

#endif
