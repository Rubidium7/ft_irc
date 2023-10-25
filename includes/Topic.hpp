#ifndef TOPIC_HPP
# define TOPIC_HPP

# include <ctime>
# include <vector>
# include <iostream>

class Channel;
class Client;
typedef struct s_server_mode t_server_mode;

class Topic
{
	public:
		static void	topicCommand(int socket, Client &client,
			std::vector<std::string> args, t_server_mode &serverSettings);

	private:

		static void	_displayTopic(std::string nick, int socket, Channel &channel, bool debug);
		static bool	_channelIssues(std::string nick, int socket, std::string chan_name,
			std::vector<Channel> &channels, std::vector<std::string>::size_type	i, bool change_topic, bool debug);
		static std::string	_combineArgs(std::vector<std::string> args);
		Topic();
		Topic(const Topic &src);
		~Topic();

		Topic &operator=(const Topic &rhs);

};

#endif
