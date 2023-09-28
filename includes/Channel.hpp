/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:16:14 by tpoho             #+#    #+#             */
/*   Updated: 2023/09/26 20:53:25 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include "defines.hpp"

class Channel
{
	public:

		Channel(const std::string nameOfChannel, int socketDescriptor);
		~Channel();

		std::string	getChannelName() const;
		int			isInviteOnly() const;

	private:

		Channel();
		Channel(const Channel &src);
		Channel	&operator=(const Channel &rhs);

		std::string					_nameOfChannel;
		std::vector<int>			_channelMembers;
		t_channel_mode				_channelMode;

};

#endif
