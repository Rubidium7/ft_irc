/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 17:09:07 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/19 15:47:05 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PART_HPP
# define PART_HPP

//# include "defines.hpp"
//# include "Channel.hpp"
# include "Server.hpp"
# include <string>

class Part
{
	public:

		static void partcmd(int socket, std::string full_command, t_server_mode	&_serverSettings);
		static void partFromAllChannels(int socket, t_server_mode &_serverSettings);

	private:

		Part();
		~Part();
		Part(Part &copy_constructor);
		Part &operator=(Part &copy_assignment);

		static std::string _returnLastPartOfString(int begin, std::string full_command);
};

#endif
