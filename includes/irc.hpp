
#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <unistd.h>
# include <cctype>
# include <sys/select.h>
# include "defines.hpp"
# include "Server.hpp"
# include "Client.hpp"

//errors.c
int		print_error(t_error_code type);

//setup.c
bool	check_characters(std::string password);
int		check_port(char	*av);

#endif
