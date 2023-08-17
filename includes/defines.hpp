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

typedef enum e_error
{
	WRONG_ARGS = 1,
	NON_DIGIT_PORT,
	OUT_OF_RANGE_PORT,
	PASSWD_TOO_LONG,
	PASSWD_NON_CHAR
}	t_error_code;

#endif
