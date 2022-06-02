//
// Created by Szymon Jędras on 01/06/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//

#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <cstring>
#include <chrono>


namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std::chrono_literals;

const milliseconds hangup_time = 1000ms;

void accept(int listenfd) {
    sockaddr_in addr;
    socklen_t addr_len;
    int accepted;

    close(STDOUT_FILENO);
    close(STDIN_FILENO);

    while((accepted = accept(listenfd, (sockaddr*)&addr, &addr_len)) < 0) {
        std::cerr << "Accept failed: " << strerror(errno) << ". \n";

    }

    dup2(accepted, STDOUT_FILENO);
    dup2(accepted, STDIN_FILENO);

    close(accepted);
}

int Poll(int sockfd, int timeout) {
    pollfd fds = {
        .fd=sockfd,
        .events=POLLIN,
        .revents=0
    };

    int res = poll(&fds, 1, timeout);
    if(res < 0) {
        perror("poll error");
        std::exit(EXIT_FAILURE);
    }
    return res;
}

void handle_connection() {
    while(Poll(STDIN_FILENO, hangup_time.count())) {
        std::string s;
        std::getline(std::cin, s);
        std::cerr << s << "\n";
        std::cout << s << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Expected: " << (argc>0?argv[0]:"webserver") << " [html root dir] [server port]\n";
        std::exit(EXIT_FAILURE);
    }

    auto path = fs::path(argv[1]);
    if(!fs::exists(path)) {
        std::cerr << "Specified path doesn't exist.\n";
        std::exit(EXIT_FAILURE);
    }
    if(!fs::is_directory(path)) {
        std::cerr << "Sepcified path isn't a directory.\n";
        std::exit(EXIT_FAILURE);
    }
    chdir(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        std::cerr << "Socket failed: " << strerror(errno) << ".\n";
        std::exit(EXIT_FAILURE);
    }

    sockaddr_in addr;
    if((addr.sin_port = htons(std::strtol(argv[2], nullptr, 10))) == 0) {
        std::cerr << "port arg is not a number\n";
        std::exit(EXIT_FAILURE);
    }
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    if(bind(sockfd, (sockaddr*)&addr, sizeof(sockaddr_in))) {
        std::cerr << "Bind failed: " << strerror(errno) << ".\n";
        std::exit(EXIT_FAILURE);
    }
    if(listen(sockfd, 10)) {
        std::cerr << "Listen failed: " << strerror(errno) << ".\n";
        std::exit(EXIT_FAILURE);
    }

    close(STDOUT_FILENO);
    close(STDIN_FILENO);
    while(true) {
        accept(sockfd);
        handle_connection();
    }
}
