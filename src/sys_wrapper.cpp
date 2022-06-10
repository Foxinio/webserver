//
// Created by Szymon Jędras on 6/9/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//

#include <sys_wrapper.hpp>

#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <unistd.h>

int Write(int fd, const char* ptr, long size) {
    int res = write(fd, ptr, size);
    if(res < 0) {
        std::cerr << "write failed [" << errno << "] " << strerror(errno) << "\n";
    }
    return res;
}
