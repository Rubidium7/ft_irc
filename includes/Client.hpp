
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
		void		setId(int id);
		int			getId() const;
		void		setNick(std::string nickName);
		std::string	getNick() const;
		void		setUserName(std::string userName);
		std::string	getUserName() const;
		void		setRealName(std::string realName);
		std::string	getRealName() const;
		void		setHostName(std::string realName);
		std::string	getHostName() const;
		void		addToBuffer(std::string text);
		std::string	getBuffer() const;
		void		emptyBuffer();

	private:
		Client(const Client &src);
		Client	&operator=(const Client &rhs);

		int			_socket;
		int			_id;
		std::string	_nickName;
		std::string	_userName;
		std::string	_realName;
		std::string	_host;
		std::string	_buffer;

};

#endif
