/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:21:27 by samusanc          #+#    #+#             */
/*   Updated: 2025/05/01 20:38:06 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MATTDAEMON_HPP
# define MATTDAEMON_HPP
# include <string>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <cstdlib>

class	Tintin_reporter{
	public:
		Tintin_reporter();
		~Tintin_reporter();
		void log_info();
		void log_err();
		void log_log();

	private:
};

#endif
