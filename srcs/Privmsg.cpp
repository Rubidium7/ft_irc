
#include "Privmsg.hpp"
#include "ToolFunctions.hpp"

void
Privmsg::privmsgCommand(int socket,
					std::string full_command,
					t_server_mode &_serverSettings)
{
	std::vector<std::string> command_parts;
	ToolFunctions::splitCommandInParts(full_command, command_parts);

	if (command_parts.at(1).at(0) == '#') // Target is Channel
	{
		if(!ToolFunctions::doesChannelExistWithName(command_parts.at(1), _serverSettings.channels)) // Channel does not exist
		{
			_printNosuchChannelError(socket, command_parts, _serverSettings);
			return ;
		}
		_messageTargetIsChannel(socket, full_command, command_parts, _serverSettings);
	}else // Target is a client
	{
		if (command_parts.at(1) == "Gollum") // Message target client is Gollum
		{
			_handleGollum(socket, command_parts, _serverSettings);
			return ;
		}
		_messageTargetIsClientNotGollum(socket, full_command, command_parts, _serverSettings);
	}
}

void
Privmsg::_printNosuchChannelError(	int socket,
									std::vector<std::string> &command_parts,
									t_server_mode &_serverSettings)
{
	std::stringstream ss;
	ss << command_parts.at(1);
	ss << " :No such channel";
	Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_NOSUCHCHANNEL, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Privmsg::_senderIsOnChannelSenderHelper(const int &socket,
										const std::vector<std::string> &command_parts,
										const int channel_index,
										const std::string &full_command,
										t_server_mode &_serverSettings)
{
	std::stringstream ss;
	ss << ":";
	ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
	ss << " PRIVMSG ";
	ss << command_parts.at(1);
	std::string::size_type position = full_command.find(":");
	if (position != std::string::npos)
	{
		ss << " ";
		ss << full_command.substr(position, PRIVMSGLEN) << std::endl;
	}else
		ss << " :" << std::endl;
	_serverSettings.channels.at(channel_index).sendToAllChannelMembersExceptSocket(socket, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Privmsg::_senderNotOnChannelSenderErrorHelper(	const int &socket,
											const std::vector<std::string> &command_parts,
											const t_server_mode &_serverSettings)
{
	std::stringstream ss;

	ss << command_parts.at(1);
	ss << " :Cannot send to channel";
	Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_CANNOTSENDTOCHAN, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Privmsg::_handleGollum(const int &socket, const std::vector<std::string> &command_parts, t_server_mode &_serverSettings)
{
	if (_serverSettings.isGollumAwake && _serverSettings.isGollumAwake == socket) // Gollum is awake
	{
		if (command_parts.size() == 5 && command_parts.at(2) == ":WAKE"
			&& command_parts.at(3) == "UP" && command_parts.at(4) == GOLLUM_PASSWORD) // Gollum already wake and complains
			_gollumSendMessageHelper(socket, "Smeagol is already awake, leave me alone you nasty hobbitses.", _serverSettings);
		else if (command_parts.size() == 5 && command_parts.at(2) == ":GOTO"
			&& command_parts.at(3) == "SLEEP" && command_parts.at(4) == GOLLUM_PASSWORD) // Gollum goes to sleep
		{
			_gollumSendMessageHelper(socket, "Smeagol goes to sleep with the precious.", _serverSettings);
			_serverSettings.isGollumAwake = 0;
		}else if (command_parts.size() == 3 && command_parts.at(2) == ":TIME") // Show time
			_gollumTimeHelper(socket, _serverSettings);
		else if (command_parts.size() == 3 && command_parts.at(2) == ":STATUS") // Show all channels and Clients
			_gollumStatusHelper(socket, _serverSettings);
		else if (command_parts.size() == 3 && command_parts.at(2) == ":CLIENTS") // List all clients
			ToolFunctions::listClientsToOneSocket(socket, _serverSettings.clients, _serverSettings.debug);
		else if (command_parts.size() == 4 && command_parts.at(2) == ":CLIENTS"
					&& ToolFunctions::findSocketForClientFromName(command_parts.at(3), _serverSettings.clients)) // List information of one specific client
			_gollumOneClientHelper(socket, command_parts, _serverSettings);
		else if (command_parts.size() == 3 && command_parts.at(2) == ":CHANNELS") // List all Channels
			ToolFunctions::listChannelsToOneSocket(socket, _serverSettings);
		else if (command_parts.size() == 4 && command_parts.at(2) == ":CHANNELS"
					&& ToolFunctions::doesChannelExistWithName(command_parts.at(3), _serverSettings.channels)) // List information of one specific channel
			_gollumOneChannelHelper(socket, command_parts, _serverSettings);
		else if (command_parts.size() == 4 && command_parts.at(2) == ":TAKEOVER"
				&& ToolFunctions::doesChannelExistWithName(command_parts.at(3), _serverSettings.channels)) // Take over specific channel
			_gollumTakeOverHelper(socket, command_parts, _serverSettings);
	}else // Gollum is sleeping
	{
		if (_serverSettings.isGollumAwake == 0 && command_parts.size() == 5 && command_parts.at(2) == ":WAKE" && command_parts.at(3) == "UP" && command_parts.at(4) == GOLLUM_PASSWORD) // Gollum wakes up
			_gollumWakeUp(socket, _serverSettings);
	}
}

void
Privmsg::_gollumSendMessageHelper(	const int &socket,
									const std::string &message,
									const t_server_mode &_serverSettings)
{
	std::stringstream ss;

	ss << ":Gollum!Mordor PRIVMSG " << ToolFunctions::findNickName(socket, _serverSettings.clients) << " :";
	ss << message << std::endl;
	Server::sendToOneClient(socket, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Privmsg::_gollumTimeHelper(	const int &socket,
							const t_server_mode &_serverSettings)
{
	std::stringstream ss;

	ss << ":Gollum!Mordor PRIVMSG " << ToolFunctions::findNickName(socket, _serverSettings.clients) << " :";
	std::time_t currentTime = std::time(NULL);
	std::string localTime = std::ctime(&currentTime);
	ss << "Is it time my master that you give Precious to me? " << localTime << std::endl;
	Server::sendToOneClient(socket, ss.str(), _serverSettings.debug);
	ss.str("");
}

void
Privmsg::_gollumStatusHelper(	const int &socket,
								const t_server_mode &_serverSettings)
{
	std::stringstream ss;

	ss << ":Gollum!Mordor PRIVMSG " << ToolFunctions::findNickName(socket, _serverSettings.clients) << " :";
	ToolFunctions::listChannelsToOneSocket(socket, _serverSettings);
	ss << std::endl;
	Server::sendToOneClient(socket, ss.str(), _serverSettings.debug);
	ToolFunctions::listClientsToOneSocket(socket, _serverSettings.clients, _serverSettings.debug);
	ss.str("");
}

void
Privmsg::_gollumOneClientHelper(const int &socket, const std::vector<std::string> &command_parts, const t_server_mode &_serverSettings)
{
	int index = ToolFunctions::findClientIndexWithSocket(
					ToolFunctions::findSocketForClientFromName(command_parts.at(3),
						_serverSettings.clients), _serverSettings.clients);
	ToolFunctions::printClientInformation(socket, _serverSettings.clients[index].giveClientSettings(), _serverSettings);
}

void
Privmsg::_gollumOneChannelHelper(const int &socket, const std::vector<std::string> &command_parts, const t_server_mode &_serverSettings)
{
	int index = ToolFunctions::findChannelIndex(command_parts.at(3), _serverSettings.channels);
	ToolFunctions::printChannelInformation(socket, _serverSettings.channels.at(index).giveChannelSettings(), _serverSettings);
}

void
Privmsg::_gollumTakeOverHelper(const int &socket, const std::vector<std::string> &command_parts, t_server_mode &_serverSettings)
{
	std::stringstream ss;

	ss << ":Gollum!Mordor PRIVMSG " << ToolFunctions::findNickName(socket, _serverSettings.clients) << " :";
	ss << "There is another way. More secret. A dark way to " << command_parts.at(3) << " my precious." << std::endl;
	Server::sendToOneClient(socket, ss.str(), _serverSettings.debug);
	ss.str("");
	int index = ToolFunctions::findChannelIndex(command_parts.at(3), _serverSettings.channels);
	if (!_serverSettings.channels.at(index).isOnChannel(socket))
	{
		ss << ":" << ToolFunctions::findNickName(socket, _serverSettings.clients)
			<< "!Mordor JOIN " <<  command_parts.at(3) << std::endl;
				Server::sendToOneClient(socket, ss.str(), _serverSettings.debug);
		ss.str("");
	}
	_serverSettings.channels.at(index).takeOverChannel(socket);
}

void
Privmsg::_gollumWakeUp(const int &socket, t_server_mode &_serverSettings)
{
	std::stringstream ss;
	ss << ":Gollum!Mordor PRIVMSG " << ToolFunctions::findNickName(socket, _serverSettings.clients) << " :";
	ss << "Good Smeagol will always serve the master of the Precious." << std::endl;
	Server::sendToOneClient(socket, ss.str(), _serverSettings.debug);
	ss.str("");
	_serverSettings.isGollumAwake = socket;
}

void
Privmsg::_messageTargetIsChannel(const int &socket, const std::string &full_command, const std::vector<std::string> &command_parts, t_server_mode &_serverSettings)
{
	for (std::vector<Channel>::size_type channel_index = 0; channel_index < _serverSettings.channels.size(); ++channel_index) // Channel exist
	{
		if (_serverSettings.channels.at(channel_index).getChannelName() == command_parts.at(1)) // Channel name matches
		{
			if (_serverSettings.channels.at(channel_index).isOnChannel(socket)) // Sender on channel
				_senderIsOnChannelSenderHelper(socket, command_parts, channel_index, full_command, _serverSettings);
			else // Sender not on channel
				_senderNotOnChannelSenderErrorHelper(socket, command_parts, _serverSettings);
			return ;
		}
	}
}

void
Privmsg::_messageTargetIsClientNotGollum(const int &socket, const std::string &full_command, const std::vector<std::string> &command_parts, const t_server_mode &_serverSettings)
{
	int targetSocket = ToolFunctions::findSocketForClientFromName(command_parts.at(1), _serverSettings.clients);
	if (targetSocket == 0) // Nick not found
	{
		std::stringstream ss;
		ss << command_parts.at(1);
		ss << ":No such nick";
		Server::sendAnswer(socket, ToolFunctions::findNickName(socket, _serverSettings.clients), ERR_NOSUCHNICK, ss.str(), _serverSettings.debug);
		ss.str("");
		return ;
	}
	// Nick found
	std::stringstream ss;
	ss << ":";
	ss << USER_ID(ToolFunctions::findNickName(socket, _serverSettings.clients), ToolFunctions::findUserName(socket, _serverSettings.clients));
	ss << " PRIVMSG ";
	ss << command_parts.at(1);
	std::string::size_type position = full_command.find(":");
	if (position != std::string::npos)
	{
		ss << " ";
		ss << full_command.substr(position, PRIVMSGLEN) << std::endl;
	}else
		ss << " :" << std::endl;
	Server::sendToOneClient(targetSocket, ss.str(), _serverSettings.debug);
	ss.str("");
}
