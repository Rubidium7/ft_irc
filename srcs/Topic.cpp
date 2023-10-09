

#include "Topic.hpp"
#include "irc.hpp"

bool	Topic::_channelIssues(std::string nick, int socket, std::string chan_name,
	std::vector<Channel> &channels, std::vector<std::string>::size_type	i, bool change_topic)
{
	if (i == channels.size())
	{
		Server::sendAnswer(socket, nick,
			ERR_NOSUCHCHANNEL, chan_name + " :No such channel");
		return (true);
	}
	if (!channels.at(i).isOnChannel(socket))
	{
		Server::sendAnswer(socket, nick,
			ERR_NOTONCHANNEL, chan_name + " :You're not on that channel");
		return (true);
	}
	if (!channels.at(i).hasOps(socket) && channels.at(i).isTopicMode() && change_topic)
	{
		Server::sendAnswer(socket, nick,
			ERR_CHANOPRIVSNEEDED, chan_name + " :You're not channel operator");
		return (true);
	}
	return (false);
}

void	Topic::_displayTopic(std::string nick, int socket, Channel &channel)
{
	std::string	msg;

	if (channel.getTopic().empty())
	{
		Server::sendAnswer(socket, nick, RPL_NOTOPIC, channel.getChannelName() + " :No topic is set.");
		return ;
	}
	msg = channel.getChannelName();
	msg += " :";
	msg += channel.getTopic();
	Server::sendAnswer(socket, nick, RPL_TOPIC, msg);
	msg.clear();
	msg = channel.getChannelName();
	msg += " " + nick + " ";

	msg += std::time(NULL);
	Server::sendAnswer(socket, nick, RPL_TOPICTIME, msg);
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
	if (_channelIssues(client.getNick(), socket, args.at(1), serverSettings.channels, i, change_topic))
		return ;
	if (!change_topic)
		return (_displayTopic(client.getNick(), socket, serverSettings.channels.at(i)));
	std::string input = args.at(2);
	input.erase(input.begin());
	if (input.size() > 255)
		input.erase(255, std::string::npos);
	serverSettings.channels.at(i).setTopic(input);
	//serverSettings.channels.at(i).sendToAllChannelMembers
	Server::sendToOneClient(socket, USER_ID(client.getNick(),
	client.getUserName()) + " TOPIC " + serverSettings.channels.at(i).getChannelName() + " :" + input + "\r\n");
}
