#ifndef MODE_HPP
# define MODE_HPP

# include <vector>
# include <iostream>
class Client;
class Channel;

typedef struct s_server_mode t_server_mode;

class Mode
{
	public:

		static void	modeCommand(int socket, Client &client,
			std::vector<std::string> args, t_server_mode &serverSettings);

	private:
		Mode();
		Mode(const Mode &src);
		~Mode();

		Mode &operator=(const Mode &rhs);

		static bool	_channelIssues(std::string nick, int socket, std::string chan_name,
			std::vector<Channel> &channels, std::vector<std::string>::size_type i);
		static void	_modeMessage(Channel &channel, std::string user_id,
			std::string flag, std::string arg);
};

#endif