//
// Created by Szymon Jędras on 6/9/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <handle_connection.hpp>

#include <request.hpp>
#include <sys_wrapper.hpp>
#include <responses/fwd.hpp>

#include <unistd.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <vector>

namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std::chrono_literals;

const milliseconds hangup_time = 1000ms;

void handle_connection(int accepted, sockaddr_in addr) {
    const int BUFFER_SIZE = 65535;
    char* buffer = new char[BUFFER_SIZE];
    while(Poll(accepted, hangup_time.count())) {
        int count;
        if((count = read(accepted, buffer, BUFFER_SIZE-1)) < 0) {
//            std::cerr << "Read failed: [" << errno << "] " << strerror(errno) << ".\n";
            break;
        }
        if(count == 0)
            continue;
        buffer[count] = 0;
        try {
            request current{buffer, count};

            std::string host = current["Host"];
            current.requested_path = std::string{host.begin(), host.begin() + host.find(':')} + "/" + current.requested_path;
            auto response = get_response(current);
            auto header = response->get_header();
            std::cout << "Requested: " << current.requested_path << "\n";
            std::cout << "find res: " << host.find(':') << "\n";
            std::cout.write(buffer, count);
            std::cout.write(header.c_str(), header.size());
            Write(accepted, header.c_str(), header.size());
            response->fill_response(accepted);
        }
        catch(request::bad_request& e) {
//            std::cerr << e.what() << "\n";
            auto response = response_not_implemented();
            auto header = response.get_header();
            Write(accepted, header.c_str(), header.size());
            break;
        }
    }
    delete [] buffer;
}

int Poll(int sockfd, int timeout) {
    pollfd fds = {
        .fd=sockfd,
        .events=POLLIN|POLLRDHUP,
        .revents=0
    };

    int res = poll(&fds, 1, timeout);
    if(res < 0) {
        perror("poll error");
        std::exit(EXIT_FAILURE);
    }
    if(res != 0 && (fds.revents&POLLIN) != 0) return true;
    return false;
}

std::unique_ptr<response> get_response(request& req) {
    if(is_forbidden(req.requested_path))
        return std::make_unique<response_forbidden>(req);
    if(fs::is_directory(req.requested_path)) {
        req.requested_path = req.requested_path + "/" + "index.html";
        return std::make_unique<response_moved_permanently>(req);
    }
    if(!fs::exists(req.requested_path))
        return std::make_unique<response_not_found>(req);
    return std::make_unique<response_ok>(req);
}

bool is_forbidden(const std::string &path) {
    int depth = -1;
    for(int i = 0; i < (int)path.size()-1 && depth >= 0; i++) {
        if(path[i] == '.' && path[i+1] == '.') depth--;
        if(path[i] == '/') depth++;
    }
    return depth<0;
}

//void copy_file(const char* file_path) {
//    int filefd = open(req.requested_path.c_str(), O_RDONLY);
//    if(filefd >= 0) {
//        const int BUFFER_SIZE = 4096;
//        char* buffer = new char[BUFFER_SIZE];
//        while (true) {
//            int red = read(filefd, buffer, BUFFER_SIZE);
//            if(red <= 0)
//                break;
//            std::cout.write(buffer)
//        }
//        close(filefd);
//    }
//}
