#ifndef PARSER_HPP
# define PARSER_HPP

# include <vector>
# include <iostream>
# include <sstream>

class Parser
{
	public:
		Parser(std::string input);
		~Parser();

		bool	isEndOfMessage();
		void	parse();
		std::string	getCommand() const;

	private:
		Parser();
		Parser(const Parser &src);
		Parser &operator=(const Parser &rhs);

		std::string					_input;
		std::string					_command;
		std::vector<std::string>	_args;

};

#endif
