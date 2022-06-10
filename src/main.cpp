//
// Created by Szymon Jędras on 01/06/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//

#include <handle_connection.hpp>

#include <iostream>
#include <cstring>
#include <filesystem>
#include <unistd.h>

namespace fs = std::filesystem;

bool is_forbidden(const std::string &path);

std::pair<sockaddr_in,int> accept(int listenfd) {
    sockaddr_in addr{};
    socklen_t addr_len = sizeof(sockaddr_in);
    int accepted;

    while((accepted = accept(listenfd, (sockaddr*)&addr, &addr_len)) < 0) {
        std::cerr << "Accept failed: [" << errno << "] " << strerror(errno) << ". \n";
    }

    return {addr,accepted};
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

    sockaddr_in addr{};
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
    std::cerr << "Running on port: " << ntohs(addr.sin_port) << "\n";

    while(true) {
        auto [addr, accepted] = accept(sockfd);
        std::cerr << "---------- Accepted ----------\n";
        handle_connection(accepted, addr);
        close(accepted);
        std::cerr << "---------- Closed ----------\n";
    }
}
