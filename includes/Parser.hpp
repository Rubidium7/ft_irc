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
		static t_mode				identifyMode(std::string input);
		void						parseCap();
		void						parseJoin();
		void						parseInvite();
		void						parseNick();
		void						parseUser();
		void						parsePass();
		void						parsePart();
		void						parseQuit();
		void						parseWhoIs();
		void						parseKick();
		void						parseTopic();
		void						parsePrivmsg();
		void						parseMode(std::string nick);
		void						parsePing(std::string serverName);

	private:
		Parser();
		Parser(const Parser &src);
		Parser &operator=(const Parser &rhs);

		void						_assignParserMessage(t_code code, std::string msg);
		bool						_hasWeirdChars(std::string str);
		bool						_isChannelKeyFormatCorrect(size_t amountOfChannels);
		bool						_isChannelFormatCorrect(size_t *amountOfChannels);
		void						_saveArguments(std::string input);
		std::vector<std::string>	_createVector(std::string string, char separator);
		bool						_onlyNumeric(std::string input);

		std::vector<std::string>	_args;
		t_message					_message;
};

#endif
