
struct Client {
    int id;
    char msg[290000];
};

Client clients[1024];
int max_fd = 0, gid = 0;
char send_buffer[300000], recv_buffer[300000];
fd_set write_set, read_set, current;

void err(const std::string& msg) {
    if (!msg.empty()) {
        std::cerr << msg << std::endl;
    } else {
        std::cerr << "Fatal error" << std::endl;
    }
    exit(1);
}

void send_to_all(int except) {
    for (int fd = 0; fd <= max_fd; fd++) {
        if (fd != except && FD_ISSET(fd, &write_set)) {
            if (send(fd, send_buffer, strlen(send_buffer), 0) == -1) {
                err("");
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        err("Wrong number of arguments");
    }
    
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) err("");
    max_fd = sockfd;
    
    // Enable port reuse
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        err("setsockopt(SO_REUSEADDR) failed");
    }
    
    FD_ZERO(&current);
    FD_SET(sockfd, &current);
    std::memset(clients, 0, sizeof(clients));
    std::memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
    servaddr.sin_port = htons(std::stoi(argv[1]));

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) != 0 || listen(sockfd, 100) != 0) {
        err("");
    }
    
    while(true) {
        read_set = write_set = current;
        if (select(max_fd + 1, &read_set, &write_set, nullptr, nullptr) == -1) continue;

        for (int fd = 0; fd <= max_fd; fd++) {
            if (FD_ISSET(fd, &read_set)) {
                if (fd == sockfd) {
                    int clientfd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
                    if (clientfd == -1) continue;
                    if (clientfd > max_fd) max_fd = clientfd;
                    clients[clientfd].id = gid++;
                    FD_SET(clientfd, &current);
                    std::sprintf(send_buffer, "server: client %d just arrived\n", clients[clientfd].id);
                    send_to_all(clientfd); 
                } else {
                    int ret = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
                    if (ret <= 0) {
                        std::sprintf(send_buffer, "server: client %d just left\n", clients[fd].id);
                        send_to_all(fd);
                        FD_CLR(fd, &current);
                        close(fd);
                        std::memset(clients[fd].msg, 0, sizeof(clients[fd].msg));
                    } else {
                        // Ensure null termination of received data for printing
                        recv_buffer[ret] = '\0';
                        std::cout << "Received from client " << clients[fd].id << ": " << recv_buffer;
                        
                        for (int i = 0, j = strlen(clients[fd].msg); i <= ret; i++, j++) {
                            clients[fd].msg[j] = recv_buffer[i];
                            if (clients[fd].msg[j] == '\n') {
                                clients[fd].msg[j] = '\0';
                                std::sprintf(send_buffer, "client %d: %s\n", clients[fd].id, clients[fd].msg);
                                send_to_all(fd);
                                std::memset(clients[fd].msg, 0, sizeof(clients[fd].msg));
                                j = -1;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    return 0;
}
