
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>

class Client
{
	public:
		Client();
		~Client();

		int		getSocket() const;
		void	setSocket(int socket);
		void	setId(int id);
		int		getId() const;
		void	runHandShake();

	private:
		Client(const Client &src);
		Client	&operator=(const Client &rhs);
		
		int			_socket;
		int			_id;
		std::string	_nickName;
		char		_buffer[MSG_SIZE];

};

#endif
