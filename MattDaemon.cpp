/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MattDaemon.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samusanc <samusanc@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:23:41 by samusanc          #+#    #+#             */
/*   Updated: 2025/05/01 22:03:22 by samusanc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MattDaemon.hpp"
#include <system_error>

Tintin_reporter::Tintin_reporter() {
	this->open_log();
	this->open_lock();
	this->info("Started.");
}

Tintin_reporter::~Tintin_reporter() {
	this->delete_lock();
	this->_log_file.close();
	this->info("Quitting.");
}

void	Tintin_reporter::delete_lock() {
	std::error_code	ec;
	std::filesystem::remove("/var/lock/matt_daemon.lock", ec);
}
void	Tintin_reporter::open_lock() {
	const std::string filename = "/var/lock/matt_daemon.lock";
	std::filesystem::path p{filename};

	if (std::filesystem::exists(filename))
	{
		this->err("Error file locked.");
        std::cout << "File already exists. Exiting.\n";
		exit(1);
    }

	std::error_code ec;
	std::filesystem::create_directories(p.parent_path(), ec);
	if (ec)
	{
		this->err("Failed to create lock directory.");
		std::cerr << "Failed to create lock directory "
				  << p.parent_path() << ": " << ec.message() << "\n";
		exit(1);
	}

	std::ofstream file(filename, std::ios::app);
	if (!file)
	{
		this->err("Error cannot create lock.");
		std::cerr << "Can't open lock file " << filename << std::endl;
		exit(1);
	}
	file.close();
}

void	Tintin_reporter::open_log() {
	const std::string filename = "/var/log/matt_daemon/matt_daemon.log";
	std::filesystem::path p{filename};

	std::error_code ec;
	std::filesystem::create_directories(p.parent_path(), ec);
	if (ec)
	{
		std::cerr << "Failed to create log directory "
				  << p.parent_path() << ": " << ec.message() << "\n";
		exit(1);
	}

	_log_file.open(filename, std::ios::app);
	if (!_log_file.is_open())
	{
		std::cerr << "Can't open log file " << filename << std::endl;
		exit(1);
	}
}

void	Tintin_reporter::info(std::string str) {
	this->log_def(str, INFO);
}

void	Tintin_reporter::err(std::string str) {
	this->log_def(str, ERR);
}

void	Tintin_reporter::log(std::string str) {
	this->log_def(str, LOG);
}

void	Tintin_reporter::quit() {
	this->info("Matt_daemon: Request quit.");
	exit(0);
}

std::string	Tintin_reporter::date() {
    std::time_t now = std::time(0);
    std::tm* t = std::localtime(&now);
    
    std::ostringstream oss;
    oss << "[" 
        << std::setfill('0') << std::setw(2) << t->tm_mday << "/"
        << std::setfill('0') << std::setw(2) << (t->tm_mon + 1) << "/"
        << (t->tm_year + 1900) << "-"
        << std::setfill('0') << std::setw(2) << t->tm_hour << ":"
        << std::setfill('0') << std::setw(2) << t->tm_min << ":"
        << std::setfill('0') << std::setw(2) << t->tm_sec << "]";
    
    return oss.str();
}

void	Tintin_reporter::log_def(std::string &str, t_log flag) {
	std::stringstream	result;

	result << this->date() << " [ ";
	switch (flag) {
		case INFO:
			result << "INFO";
			break;
		case ERR:
			result << "ERROR";
			break;
		case LOG:
			result << "LOG";
			break;
		default:
			result << "?";
			break;
	}
	result << " ] - Matt_daemon: " << str << std::endl;
	_log_file << result.str();
    _log_file.flush();
}
