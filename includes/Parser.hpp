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

		t_code						getMessageCode() const;
		t_message					getMessage() const;
		std::vector<std::string>	getArgs() const;
		std::string					getCommand() const;
		void						parseCap();
		void						parseJoin();

	private:
		Parser();
		Parser(const Parser &src);
		Parser &operator=(const Parser &rhs);

		void	_assignParserMessage(t_code code, std::string msg);
		void	_saveArguments(std::string input);

		std::vector<std::string>	_args;
		t_message					_message;
};

#endif
