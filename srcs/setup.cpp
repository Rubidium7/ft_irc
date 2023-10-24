
#include "irc.hpp"

bool	check_characters(std::string password)
{
	std::string::size_type pwd_size = password.size();
 
	if (!pwd_size)
		return (true);
	for (std::string::size_type i = 0; i < pwd_size; ++i)
	{
		if (!isalnum(password.at(i)))
			return (true);
	}
	return (false);
}

int	check_port(char	*av)
{
	for (int i = 0; av[i]; i++)
	{
		if (av[i] < '0' || av[i] > '9')
			return (print_error(NON_DIGIT_PORT));
		if (i > 5)
			return (print_error(OUT_OF_RANGE_PORT));
	}
	
	std::stringstream transform;
	std::string number(av);
	transform << number;
	int port_number = 0;
	if (!(transform >> port_number))
		return (print_error(NON_DIGIT_PORT));
	if (port_number < 1024 || port_number > 49151)
		return (print_error(OUT_OF_RANGE_PORT));
	return (port_number);
}
