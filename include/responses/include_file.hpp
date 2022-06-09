//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <string>

class include_file {
    std::string& path;
public:
    include_file(std::string& path);

    friend std::ostream& operator<<(std::ostream& stream, const include_file&);
    int get_size();
};

