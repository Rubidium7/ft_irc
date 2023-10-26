
#ifndef KICK_HPP
# define KICK_HPP

# include "Server.hpp"

typedef struct s_kickcmd_data
{
	std::vector<std::string>	command_parts;
	std::vector<std::string>	temp_channels;
	std::vector<std::string>	temp_users;
	int							socket;
	std::string					full_command;
}	t_kickcmd_data;

class Kick
{
	public:

		static void kickCommand(int socket, std::string full_command, t_server_mode	&_serverSettings);

	private:

		Kick();
		~Kick();
		Kick(Kick &copy_constructor);
		Kick &operator=(Kick &copy_assignment);

		static void _printDoesChannelExistError(const t_kickcmd_data &data,
			std::vector<std::string>::size_type &j, t_server_mode &_serverSettings);
		static void _printYoureNotChannelOperatorError(t_kickcmd_data &data,
			std::vector<std::string>::size_type &j, t_server_mode &_serverSettings);
		static void _printUserIsNotOnThatChannelError(const t_kickcmd_data &data,
			std::vector<std::string>::size_type &j, t_server_mode &_serverSettings);
		static void _kickUserFromChannel(const t_kickcmd_data &data, std::vector<Channel>::size_type &i,
			std::vector<std::string>::size_type &k, t_server_mode &_serverSettings);
		static void	_goThroughTempUsersLoopHelper(const t_kickcmd_data &data, std::vector<std::string>::size_type &j,
			std::vector<Channel>::size_type &i, t_server_mode &_serverSettings);
};

#endif
