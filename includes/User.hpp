#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <vector>

class Client;

class User
{
	public:
		static bool userCommand(int socket, Client &client, std::vector<std::string> args);

	private:
		static std::string	_parseRealName(std::vector<std::string> args);

		User();
		User(const User &src);
		~User();

		User &operator=(const User &rhs);
};

#endif
