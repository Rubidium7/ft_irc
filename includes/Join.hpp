/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:17:30 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/02 17:57:43 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_HPP
# define JOIN_HPP

# include "defines.hpp"
//# include "Channel.hpp"
# include "Server.hpp"
# include <string>

class Join
{
	public:

		static void joincmd(int socket, std::string full_command, t_server_mode	&_serverSettings);

	private:

		Join();
		~Join();
		Join(Join &copy_constructor);
		Join &operator=(Join &copy_assignment);
};

#endif
