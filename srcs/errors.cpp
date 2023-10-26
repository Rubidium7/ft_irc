
#include "irc.hpp"

int	print_error(t_error_code type)
{
	if (type == WRONG_ARGS)
		std::cerr << "[USAGE]: ./ircserv <port> <password> optional: debug" << std::endl;
	if (type == PASSWD_TOO_LONG)
		std::cerr << "password has to be 100 characters or less" << std::endl;
	if (type == PASSWD_NON_CHAR)
		std::cerr << "password can only include letters or digits" << std::endl;
	if (type == NON_DIGIT_PORT)
		std::cerr << "port has to include only digits" << std::endl;
	if (type == OUT_OF_RANGE_PORT)
		std::cerr << "port has to be between the numbers 1024 and 49151" << std::endl;
	if (type == SERV_SOCKET_FAILURE)
		std::cerr << "error with creating a socket" << std::endl;
	if (type == SERV_BIND_FAILURE)
		std::cerr << "error with binding the socket" << std::endl;
	if (type == SERV_LISTEN_FAILURE)
		std::cerr << "error with listening to a socket" << std::endl;
	if (type == SERV_SELECT_FAILURE)
		std::cerr << "error with socket monitoring" << std::endl;
	if (type == SERV_ACCEPT_FAILURE)
		std::cerr << "error with accepting a new client" << std::endl;
	return (type);
}
