
#include "Topic.hpp"
#include "irc.hpp"

bool	Topic::_channelIssues(std::string nick, int socket, std::string chan_name,
	std::vector<Channel> &channels, std::vector<std::string>::size_type	i, bool change_topic, bool debug)
{
	if (i == channels.size())
	{
		Server::sendAnswer(socket, nick,
			ERR_NOSUCHCHANNEL, chan_name + " :No such channel", debug);
		return (true);
	}
	if (!channels.at(i).isOnChannel(socket))
	{
		Server::sendAnswer(socket, nick,
			ERR_NOTONCHANNEL, chan_name + " :You're not on that channel", debug);
		return (true);
	}
	if (!channels.at(i).hasOps(socket) && channels.at(i).isTopicMode() && change_topic)
	{
		Server::sendAnswer(socket, nick,
			ERR_CHANOPRIVSNEEDED, chan_name + " :You're not channel operator", debug);
		return (true);
	}
	return (false);
}

void	Topic::_displayTopic(std::string nick, int socket, Channel &channel, bool debug)
{
	std::string			msg;
	std::string			time_str;
	std::stringstream	ss;

	if (channel.getTopic().empty())
	{
		Server::sendAnswer(socket, nick, RPL_NOTOPIC, channel.getChannelName() + " :No topic is set.", debug);
		return ;
	}
	msg = channel.getChannelName();
	msg += " :";
	msg += channel.getTopic();
	Server::sendAnswer(socket, nick, RPL_TOPIC, msg, debug);
	msg.clear();
	msg =  channel.getChannelName();
	msg += " " + nick + " ";

	time_t time = std::time(NULL);
	ss << time;
	ss >> time_str;
	Server::sendAnswer(socket, nick, RPL_TOPICTIME, msg + time_str, debug);
}

std::string	Topic::_combineArgs(std::vector<std::string> args)
{
	std::string	combine;

	combine = args.at(2);
	for (size_t i = 3; i < args.size(); i++)
	{
		combine += " ";
		combine += args.at(i);
	}
	return (combine);
}

void	Topic::topicCommand(int socket, Client &client,
	std::vector<std::string> args, t_server_mode &serverSettings)
{
	std::vector<std::string>::size_type	i;
	bool	change_topic = false;

	if (args.size() > 2)
		change_topic = true;
	for (i = 0; i < serverSettings.channels.size(); i++)
	{
		if (serverSettings.channels.at(i).getChannelName() == args.at(1))
			break ;
	}
	if (_channelIssues(client.getNick(), socket, args.at(1), serverSettings.channels, i, change_topic, serverSettings.debug))
		return ;
	if (!change_topic)
		return (_displayTopic(client.getNick(), socket, serverSettings.channels.at(i), serverSettings.debug));
	std::string input = _combineArgs(args);
	input.erase(input.begin());
	if (input.size() > TOPICLEN)
		input.erase(TOPICLEN, std::string::npos);
	serverSettings.channels.at(i).setTopic(input);
	serverSettings.channels.at(i).sendToAllChannelMembers(":" + USER_ID(client.getNick(),
	client.getUserName()) + " TOPIC " + serverSettings.channels.at(i).getChannelName() + " :" + input + "\r\n",
		serverSettings.debug);
}
