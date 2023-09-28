
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>

class Client
{
	public:
		Client();
		~Client();

		int			getSocket() const;
		void		setSocket(int socket);

		std::string	getNickName() const;
		void		setNickName(std::string &name);

	private:
		Client(const Client &src);
		Client	&operator=(const Client &rhs);
		
		int			_socket;
		std::string	_nickName;

};

#endif
