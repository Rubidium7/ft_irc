#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <vector>

class Client;

class User
{
	public:
		static void userCommand(int socket, Client &client, std::vector<std::string> args, bool debug);

	private:
		static bool			_weirdCharsInRealName(int socket, std::string nick, bool debug, std::vector<std::string> args);
		static bool			_weirdChars(std::string &name);
		static std::string	_parseRealName(std::vector<std::string> args);

		User();
		User(const User &src);
		~User();

		User &operator=(const User &rhs);
};

#endif
