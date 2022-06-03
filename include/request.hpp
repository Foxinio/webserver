//
// Created by Szymon Jędras on 01/06/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <map>

class request {
    std::map<std::string,std::string> data;
    std::string requested_path;
public:
    request(char* buffer, int size);

    auto operator[](const std::string& arg);
    void add_options(char * buffer, int size);

    class bad_request : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

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

    void initialize_path(char *begin, char *end);
    int get_index(char *ptr, char to_find);
    bool is_request_get(char *buffer, int size);
};

