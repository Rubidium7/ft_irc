/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlonka <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 11:38:29 by nlonka            #+#    #+#             */
/*   Updated: 2023/08/17 14:46:52 by nlonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_HPP
# define DEFINES_HPP

# define MAX_AMOUNT_CLIENTS 1023

typedef enum e_error
{
	NO_ERROR,
	WRONG_ARGS,
	NON_DIGIT_PORT,
	OUT_OF_RANGE_PORT,
	PASSWD_TOO_LONG,
	PASSWD_NON_CHAR,
	SERV_SOCKET_FAILURE,
	SERV_BIND_FAILURE,
	SERV_LISTEN_FAILURE
}	t_error_code;

#endif
