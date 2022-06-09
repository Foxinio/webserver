//
// Created by Szymon Jędras on 6/9/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include "stream.hpp"

#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <cassert>

inline char stream::getc() {
    if(begin == end) {
        int to_read = BUFFER_SIZE - end;
        int read = recv(fd, buffer+end, to_read, MSG_DONTWAIT);
        if(read < 0) {
            std::cerr << "Listen failed: " << strerror(errno) << ".\n";
            std::exit(EXIT_FAILURE);
        }
        end += read;
        assert(end <= BUFFER_SIZE);
        end %= BUFFER_SIZE;
    }
    char result = buffer[begin];
    begin = (++begin)%BUFFER_SIZE;
    return result;
}

stream::operator bool() {
    if(begin == end) {
        int to_read = BUFFER_SIZE - end;
        int read = recv(fd, buffer+end, to_read, MSG_DONTWAIT);
        if(read < 0) {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
                return false;
            else {
                std::cerr << "Listen failed: " << strerror(errno) << ".\n";
                std::exit(EXIT_FAILURE);
            }
        }
        end += read;
        assert(end <= BUFFER_SIZE);
        end %= BUFFER_SIZE;
        return true;
    }
    return true;
}

int stream::get_n(char *ptr, int n) {
    int to_read = std::min(n, begin <= end ? end - begin : BUFFER_SIZE - begin);
    if(to_read == 0) return 0;
    std::memcpy(ptr, buffer+begin, to_read);
    return to_read;
}

void stream::getline(std::string &s) {
    bool cr = false;
    s.clear();
    while(*this) {
        char c = getc();
        if(c == '\n') break;
        if(cr) {
            s.push_back('\r');
            cr = false;
        }
        if(c == '\r')
            cr = true;
        s.push_back(c);
    }
}
