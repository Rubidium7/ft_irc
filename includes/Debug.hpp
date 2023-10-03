/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 17:18:36 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/03 17:32:42 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_HPP
# define DEBUG_HPP

//# include "defines.hpp"
//# include "Channel.hpp"
# include "Server.hpp"
# include <string>

class Debug
{
	public:

		static void debugcmd(int socket, std::string full_command, t_server_mode &_serverSettings);

	private:

		Debug();
		~Debug();
		Debug(Debug &copy_constructor);
		Debug &operator=(Debug &copy_assignment);

		//static std::string _return_last_part_of_string(int begin, std::string full_command);
};

#endif
