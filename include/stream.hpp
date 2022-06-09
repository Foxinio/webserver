//
// Created by Szymon Jędras on 6/9/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once


#include <string>

class stream {
    static constexpr int BUFFER_SIZE = 2048;

    char buffer[BUFFER_SIZE];
    int fd;

    /*
     * points to character in buffer to be read,
     *      is always smaller than BUFFER_SIZE
     */
    int begin;
    /*
     * points to first empty place in the buffer,
     *      if is equal to begin then buffer is empty,
     *      is always smaller than BUFFER_SIZE
     */
    int end;

public:
    inline char getc();
    inline int get_n(char* ptr, int n);

    operator bool();

    void getline(std::string& s);
};
