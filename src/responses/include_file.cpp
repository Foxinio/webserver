//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include "responses/include_file.hpp"

#include <fstream>
#include <filesystem>

include_file::include_file(std::string &path)
    : path(path) {}

std::ostream &operator<<(std::ostream &stream, const include_file & obj) {
    std::ifstream file{obj.path};
    if(file.is_open()) {
        const int BUFFER_SIZE = 512;
        char buffer[BUFFER_SIZE];
        while (!file.eof()) {
            file.read(buffer, BUFFER_SIZE);
            stream.write(buffer, BUFFER_SIZE);
        }
        file.close();
    }
    return stream;
}

int include_file::get_size() {
    return std::filesystem::file_size({path});
}
