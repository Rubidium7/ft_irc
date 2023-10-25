
#ifndef TOOLFUNCTIONS_HPP
# define TOOLFUNCTIONS_HPP

# include "Server.hpp"

class ToolFunctions
{
	public:

		static void 		splitCommandInParts(const std::string &full_command,
			std::vector<std::string> &command_parts);
		static void 		parseIntoParts(const std::vector<std::string> &command_parts,
			int part, std::vector<std::string> &temp_strings);
		static std::string	findNickName(const int socket, const Client clients[]);
		static std::string	findUserName(const int socket, const Client clients[]);
		static int			findSocketForClientFromName(const std::string nick,
			const Client *clients);
		static int			findChannelIndex(const std::string channelName,
			const std::vector<Channel> &channels);
		static int			findClientIndexWithSocket(const int socket, const Client clients[]);
		static int			doesChannelExistWithName(const std::string &nameChannel,
			const std::vector<Channel> &channels);
		static void			listChannelsToOneSocket(int socket, const t_server_mode &_serverSettings);
		static void			listClientsToOneSocket(int socket, const Client clients[], bool debug);
		static void			printClientInformation(int socket,
			const t_client_mode &_clientSettings, const t_server_mode &_serverSettings);
		static void			printChannelInformation(int socket,
			const t_channel_mode &_channelSettings, const t_server_mode &_serverSettings);

	private:

		ToolFunctions();
		~ToolFunctions();
		ToolFunctions(ToolFunctions &copy_constructor);
		ToolFunctions &operator=(ToolFunctions &copy_assignment);
};

#endif
