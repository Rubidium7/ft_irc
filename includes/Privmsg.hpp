
#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include "Server.hpp"

class Privmsg
{
	public:

		static void privmsgCommand(int socket, std::string full_command, t_server_mode	&_serverSettings);

	private:

		Privmsg();
		~Privmsg();
		Privmsg(Privmsg &copy_constructor);
		Privmsg &operator=(Privmsg &copy_assignment);

		static void _printNosuchChannelError(int socket, std::vector<std::string> &command_parts,
			t_server_mode &_serverSettings);
		static void _senderIsOnChannelSenderHelper(const int &socket, const std::vector<std::string> &command_parts,
			const int channel_index, const std::string &full_command, t_server_mode &_serverSettings);
		static void _senderNotOnChannelSenderErrorHelper(const int &socket,
			const std::vector<std::string> &command_parts, const t_server_mode &_serverSettings);
		static void _handleGollum(const int &socket, const std::vector<std::string> &command_parts,
			t_server_mode &_serverSettings);
		static void	_gollumSendMessageHelper(const int &socket, const std::string &message,
			const t_server_mode &_serverSettings);
		static void	_gollumTimeHelper(const int &socket, const t_server_mode &_serverSettings);
		static void _gollumStatusHelper(const int &socket, const t_server_mode &_serverSettings);
		static void _gollumOneClientHelper(const int &socket, const std::vector<std::string> &command_parts,
			const t_server_mode &_serverSettings);
		static void _gollumOneChannelHelper(const int &socket, const std::vector<std::string> &command_parts,
			const t_server_mode &_serverSettings);
		static void _gollumTakeOverHelper(const int &socket, const std::vector<std::string> &command_parts,
			t_server_mode &_serverSettings);
		static void _gollumWakeUp(const int &socket, t_server_mode &_serverSettings);
		static void _messageTargetIsChannel(const int &socket, const std::string &full_command,
			const std::vector<std::string> &command_parts, t_server_mode &_serverSettings);
		static void _messageTargetIsClientNotGollum(const int &socket, const std::string &full_command,
			const std::vector<std::string> &command_parts, const t_server_mode &_serverSettings);
};

#endif
