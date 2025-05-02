#include "Server.hpp"

Server::Server(Tintin_reporter* reporter):fatal(false), reporter(reporter), max_fd(0), gid(0), client_count(0), len(sizeof(socklen_t)){
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1) 
        this->reporter->err("");
    this->max_fd = this->sockfd;
    
    // Enable port reuse
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        this->reporter->err("setsockopt(SO_REUSEADDR) failed");
        this->fatal =true;
        return;
    }
    
    FD_ZERO(&(this->current));
    FD_SET(this->sockfd, &(this->current));
    std::memset(this->clients, 0, sizeof(this->clients));
    std::memset(&(this->servaddr), 0, sizeof(this->servaddr));

    this->servaddr.sin_family = AF_INET; 
    this->servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
    this->servaddr.sin_port = htons(4242);

    if (bind(this->sockfd, (const struct sockaddr *)&(this->servaddr), sizeof((this->servaddr))) != 0 || listen(sockfd, 100) != 0) {
        this->reporter->err("");
        this->fatal =true;
        return;
    }
} 


Server::~Server(){
    
}