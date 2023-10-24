/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:17:30 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/24 15:53:29 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_HPP
# define JOIN_HPP

# include "Server.hpp"

class Join
{
	public:

		static void joinCommand(const int &socket, std::string full_command,
			t_server_mode	&_serverSettings);

	private:

		Join();
		~Join();
		Join(Join &copy_constructor);
		Join &operator=(Join &copy_assignment);

		static int	_handleSpecialCases(const int &socket, const std::vector<std::string> &command_parts,
			t_server_mode &_serverSettings);
		static void _channelDoesNotExistHelper(const int &socket, const std::string &full_command,
			const std::vector<std::string>::size_type &i, const std::vector<std::string> &temp_channels,
				const std::vector<std::string> &temp_keys, t_server_mode &_serverSettings);
		static void _channelDoesNotExistHelper(const int &socket, const std::string &full_command,
			const std::vector<std::string>::size_type &i, const std::vector<std::string> &temp_channels,
				t_server_mode &_serverSettings);
		static void	_inviteOnlyErrorHelper(const int &socket, const std::vector<Channel>::size_type	&k,
			t_server_mode &_serverSettings);
		static void _keyMatchesHelper(const int &socket, const std::string &full_command,
			const std::vector<Channel>::size_type &k, t_server_mode	&_serverSettings);
		static void _keyDoesNotMatchHelper(const int &socket, const std::vector<Channel>::size_type	&k,
		t_server_mode &_serverSettings);
		static void _clientDoesNotProvideKeyErrorHelper(const int &socket, const std::vector<Channel>::size_type &k,
			const t_server_mode &_serverSettings);
		static void _channelDoesNotHaveKeyHelper(const int &socket, const std::vector<Channel>::size_type &k,
			const std::string &full_command, t_server_mode &_serverSettings);
};

#endif
