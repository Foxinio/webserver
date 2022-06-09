//
// Created by Szymon Jędras on 6/9/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//

#include <sys_wrapper.hpp>

#include <sys/types.h>
#include <fstream>
#include <unistd.h>

int Write(int fd, const char* ptr, long size) {
    return write(fd, ptr, size);
}
