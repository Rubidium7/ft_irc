#ifndef PARSER_HPP
# define PARSER_HPP

# include "irc.hpp"
# include <vector>
# include <iostream>
# include <sstream>

class Parser
{
	public:
		Parser(std::string full_command);
		~Parser();


		void						parse();
		t_code						getMessageCode() const;
		t_message					getMessage() const;
		std::vector<std::string>	getArgs() const;
		std::string					getCommand() const;

		friend class	Server;

	private:
		Parser();
		Parser(const Parser &src);
		Parser &operator=(const Parser &rhs);

		void	_assignMessage(t_code code, std::string msg);
		void	_saveArguments(std::string input);
		void	_parseCap();
		void	_parseJoin();

		std::vector<std::string>	_args;
		t_message					_message;
};

#endif
