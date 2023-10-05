/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ToolFunctions.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 17:53:10 by tpoho             #+#    #+#             */
/*   Updated: 2023/10/05 16:23:09 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOLFUNCTIONS_HPP
# define TOOLFUNCTIONS_HPP

//# include "defines.hpp"
//# include "Channel.hpp"
# include "Server.hpp"
# include <string>

class ToolFunctions
{
	public:

		static void 		_split_command_in_parts(const std::string &full_command, std::vector<std::string> &command_parts);
		static void 		_parse_into_parts(const std::vector<std::string> &command_parts, int part, std::vector<std::string> &temp_strings);
		static std::string	_findNickName(const int socket, const Client clients[]);

	private:

		ToolFunctions();
		~ToolFunctions();
		ToolFunctions(ToolFunctions &copy_constructor);
		ToolFunctions &operator=(ToolFunctions &copy_assignment);
};

#endif