
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpoho <tpoho@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:16:14 by tpoho             #+#    #+#             */
/*   Updated: 2023/09/29 16:40:41 by tpoho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>

typedef struct s_channel_mode
{
	std::string			name_of_channel;	// Name of channel
	std::vector<int>	channel_members;	// Who are on channel
	int					invite_only;		// Invite only channel
	int					topic;				// Restrict topic to only ops
	std::string			key;				// channel key (password)
	std::vector<int>	ops_list;			// Who has the ops 
	int					limit_users;		// limit of users
}	t_channel_mode;

class Channel
{
	public:

		Channel(const std::string nameOfChannel, int socketDescriptor);
		~Channel();

		std::string	getChannelName() const;
		int			isInviteOnly() const;
		int			isOnChannel(int socket) const;
		void		addToChannel(int socket);

	private:

		Channel();
		Channel(const Channel &src);
		Channel	&operator=(const Channel &rhs);

		t_channel_mode _channelSettings;
};

#endif
