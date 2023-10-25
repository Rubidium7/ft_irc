
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

typedef struct s_channel_mode
{
	std::string					nameOfChannel;	// Name of channel
	std::vector<int>			channelMembers;	// Who are on channel
	int							i;				// Is invite only channel
	std::vector<int>			invitedClients;	// List of invitations, one time only remember to delete once used
	int							t;				// Restrict topic to only ops
	std::string					topic;			// Topic that is printed when you join channel
	std::string					k;				// channel key (password)
	std::vector<int>			o;				// Who has the ops
	int							l;				// Limit of how many users can join into a channel
}	t_channel_mode;

class Channel
{
	public:

		Channel(const std::string nameOfChannel, int socketDescriptor);
		~Channel();
		Channel(const Channel &copy_constructor);
		Channel	&operator=(const Channel &copy_assignment);

		std::string				getChannelName() const;
		std::string				getTopic() const;
		void					setTopic(std::string input);
		int						isInviteOnly() const;
		int						isTopicMode() const;
		int						isClientInvited(int socket) const;
		void					addInvitation(int socket);
		void					removeInvitation(int socket);
		int						isOnChannel(int socket) const;
		void					addToChannel(int socket);
		void					partFromChannel(int socket);
		int						howManyMembersOnChannel() const;
		int						isThereKey() const;
		void					setKey(std::string new_key);
		int						doesKeyMatch(const std::string &key) const;
		void					sendToAllChannelMembers(const std::string message, bool debug);
		void					sendToAllChannelMembersExceptSocket(const int &socket, const std::string msg, bool debug);
		const std::vector<int>& returnChannelMembers() const;
		int 					hasOps(int socket) const;
		void					giveOps(int socket);
		void					removeOps(int socket);
		void					setInviteMode(int mode);
		void					setTopicMode(int mode);
		void					setUserLimit(int amount);
		int						getUserLimit() const;
		int						getLastUsersSocket() const;
		void					setNewOpIfNoOp();
		void					takeOverChannel(int socket);
		const t_channel_mode	&giveChannelSettings() const;

	private:

		Channel();

		t_channel_mode _channelSettings;
};

#endif
