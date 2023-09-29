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

class Client
{
	public:
		Client();
		~Client();

		int			getSocket() const;
		void		setSocket(int socket);

		std::string	getNick() const;
		void		setNick(std::string nickName);

	private:
		Client(const Client &src);
		Client	&operator=(const Client &rhs);
		
		int			_socket;
		std::string	_nickName;
		int			_registration;

};

#endif
