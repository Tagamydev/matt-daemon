/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:21:27 by samusanc          #+#    #+#             */
/*   Updated: 2025/05/01 22:05:00 by samusanc         ###   ########.fr       */
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
# include <sstream>
# include <iostream>
# include <fstream>
# include <filesystem>
# include <ctime>
# include <iomanip>
# include <csignal>

typedef enum	e_log{
	INFO,
	ERR,
	LOG
}	t_log;

class	Tintin_reporter{
	public:
		Tintin_reporter();
		~Tintin_reporter();
		void	info(std::string str);
		void	err(std::string str);
		void	log(std::string str);
		void	quit();
		void	delete_lock();
		
	private:
		void	open_log();
		void	open_lock();
		void	log_def(std::string &str, t_log flag);
		std::string		date();
		std::ofstream	_log_file;
		bool	lock;
};

#endif
