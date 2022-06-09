//
// Created by Szymon Jędras on 01/06/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//

#include <request.hpp>
#include <responses/fwd.hpp>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netinet/in.h>

#include <cstring>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <vector>
#include <list>

int stdin_fd = STDIN_FILENO;
int stdout_fd = STDOUT_FILENO;

namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std::chrono_literals;

const milliseconds hangup_time = 1000ms;

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

std::pair<sockaddr_in,int> flipped_accept(int listenfd) {
    sockaddr_in addr{};
    socklen_t addr_len = sizeof(sockaddr_in);
    int accepted;

    close(STDOUT_FILENO);
    close(STDIN_FILENO);

    while((accepted = accept(listenfd, (sockaddr*)&addr, &addr_len)) < 0) {
        std::cerr << "Accept failed: [" << errno << "] " << strerror(errno) << ". \n";
    }

    dup2(accepted, STDOUT_FILENO);
    dup2(accepted, STDIN_FILENO);

    close(accepted);
    return {addr,accepted};
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

std::unique_ptr<response> get_response(request& req) {
    if(is_forbidden(req.requested_path))
        return std::make_unique<response_forbidden>(req);
    if(!fs::exists(req.requested_path)) {
        if(fs::is_directory(req.requested_path) && fs::exists(req.requested_path+"/"+"index.html"))
            return std::make_unique<response_moved_permanently>(req);
        else
            return std::make_unique<response_not_found>(req);
    }
    return std::make_unique<response_ok>(req);
}

bool is_forbidden(const std::string &path) {
    int depth = 0;
    for(int i = 0; i < path.size()-1 && depth >= 0; i++) {
        if(path[i] == '.' && path[i+1] == '.') depth--;
        if(path[i] == '/') depth++;
    }
    return depth<0;
}

void handle_connection(int accepted, sockaddr_in addr) {
    const int BUFFER_SIZE = 65535;
    char* buffer = new char[BUFFER_SIZE];
    while(Poll(accepted, hangup_time.count())) {
        int count;
        if((count = read(accepted, buffer, BUFFER_SIZE-1)) < 0) {
            std::cerr << "Read failed: [" << errno << "] " << strerror(errno) << ".\n";
            continue;
        }
        if(count == 0)
            continue;
//        std::cout.write(buffer, count);
        buffer[count] = 0;
        try {
            request current{buffer, count};
            auto response = get_response(current)->get_response();
            std::cout.write(response.c_str(), response.size());
            write(accepted, response.c_str(), response.size());
        }
        catch(request::bad_request& e) {
            std::cerr << e.what();
            auto response = response_not_implemented().get_response();
            write(accepted, response.c_str(), response.size());
            break;
        }
    }
    delete [] buffer;
    close(accepted);
}

void flipped_handle() {
    while(Poll(STDIN_FILENO, hangup_time.count())) {
        std::string s;
        try {

        }
        catch(request::bad_request& e) {
            std::cerr << e.what();
            break;
        }
    }
    std::cin.clear();
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

    stdin_fd = dup(STDOUT_FILENO);
    stdout_fd = dup(STDIN_FILENO);
    while(true) {
        auto [addr, accepted] = accept(sockfd);
        std::cerr << "---------- Accepted ----------\n";
        handle_connection(accepted, addr);
//        flipped_handle();
        std::cerr << "---------- Closed ----------\n";
    }
}
