//
// Created by Szymon Jędras on 01/06/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <map>

class request {
    std::map<std::string,std::string> data;

public:
    request(char* buffer, int size);

    std::string operator[](const std::string& arg) const;

private:
    class filter {
        request& parent;
        const char* to_filer;
    public:
        filter(request& parent, const char* to_filer);
        filter& find(const char* pattern);
    private:
        bool contains(const char* pattern);
    };
    friend class filter;

    filter match(const char* line);
    void parse(const std::string &line);
};

