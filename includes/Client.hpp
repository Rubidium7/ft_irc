// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Client.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tpoho <marvin@42.fr>                       +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2023/09/29 17:37:31 by tpoho             #+#    #+#             //
//   Updated: 2023/09/29 17:37:32 by tpoho            ###   ########.fr       //
//                                                                            //
// ************************************************************************** //



#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include "defines.hpp"

class Client
{
	public:
		Client();
		~Client();

		int			getSocket() const;
		void		setSocket(int socket);

		std::string	getNick() const;
		void		setNick(std::string nickName);
		std::string	getUserName() const;
		void		setUserName(std::string userName);
		std::string	getRealName() const;
		void		setRealName(std::string realName);
		std::string	getHostName() const;
		void		setHostName(std::string hostName);
		int			getRegistrationStatus() const;
		void		setRegistrationStatus(int status);

	private:
		Client(const Client &src);
		Client	&operator=(const Client &rhs);

		int			_socket;
		std::string	_nickName;
		std::string	_userName;
		std::string	_realName;
		std::string	_hostName;
		int			_registration;

};

#endif
