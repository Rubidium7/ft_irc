
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "defines.hpp"

typedef struct s_client_mode
{
	int			socket;
	std::string	nickName;
	std::string	userName;
	std::string	realName;
	std::string	hostName;
	bool		givenPass;
}	t_client_mode;

class Client
{
	public:

		Client();
		~Client();

		int					getSocket() const;
		void				setSocket(int socket);
		std::string			getNick() const;
		void				setNick(std::string nickName);
		std::string			getUserName() const;
		void				setUserName(std::string userName);
		std::string			getRealName() const;
		void				setRealName(std::string realName);
		std::string			getHostName() const;
		void				setHostName(std::string hostName);
		bool				hasGivenPass() const;
		int					registrationStatus() const;
		void				setGivenPass(bool truth);
		void				clearInfo();
		const t_client_mode	&giveClientSettings() const;

	private:
		Client(const Client &src);
		Client	&operator=(const Client &rhs);

		t_client_mode _clientSettings;
};

#endif
