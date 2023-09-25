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
		std::vector<std::string>	getArgs() const;

	private:
		Parser();
		Parser(const Parser &src);
		Parser &operator=(const Parser &rhs);

		void	_saveArguments();
		void	_parseCap();
		void	_parseJoin();
		void	_emptyCommand();

		std::string					_commandOptions[10];
		std::string					_input;
		std::string					_wholeInput;
		std::string					_command;
		std::vector<std::string>	_args;

};

#endif
