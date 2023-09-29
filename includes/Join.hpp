/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 16:17:30 by tpoho             #+#    #+#             */
/*   Updated: 2023/09/29 17:03:38 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_HPP
# define JOIN_HPP

# include "Channel.hpp"
# include <string>

class Join
{
	public:
		static void joincmd(int id, std::string full_command, std::vector<Channel> &_channels);

	private:

		Join();
		~Join();
		Join(Join &copy_constructor);
		Join &operator=(Join &copy_assignment);

		static void _split_command_in_parts(const std::string &full_command, std::vector<std::string> &command_parts);
		static void _parse_into_parts(const std::vector<std::string> &command_parts, int part, std::vector<std::string> &temp_strings);
};

#endif
