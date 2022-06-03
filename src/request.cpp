//
// Created by Szymon Jędras on 01/06/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <request.hpp>

#include <cstring>

int request::get_index(char* ptr, char to_find) {
    auto res = strchr(ptr, to_find);
    if(res == nullptr) {
        throw bad_request("char not found");
    }
    return ptr - res;
}

bool request::is_request_get(char* buffer, int size) {
    return size > 3 &&
           buffer[0] == 'G' &&
           buffer[1] == 'E' &&
           buffer[2] == 'T';
}
request::request(char *buffer, int size) {
    if(!is_request_get(buffer, size)) throw bad_request("not a GET request");
    int j = get_index(buffer, ' ') + 1;
    int i = get_index(buffer + j, ' ');
    initialize_path(buffer + j, buffer + i);
    j = get_index(buffer+i,'\n') + 1;
    add_options(buffer + j, size - j);
}

void request::initialize_path(char* begin, char* end) {
    requested_path.reserve(end-begin);
    while(begin<end) {
        if(*begin=='%') {
            char c = 0;
            while(++begin<end &&
                    (*begin>='0' && *begin<='9' ||
                     *begin>='a' && *begin<='f' ||
                     *begin>='A' && *begin<='F')) {
                if(*begin>='0' && *begin<='9')
                    c = 16*c+(*begin-'0');
                else if(*begin>='a' && *begin<='f')
                    c = 16*c+(*begin-'a'+10);
                else if(*begin>='A' && *begin<='F')
                    c = 16*c+(*begin-'A'+10);
            }
            requested_path.push_back(c);
        }
        else {
            requested_path.push_back(*begin);
            begin++;
        }
    }
}

void request::parse(const std::string &line) {
    auto iter = std::find_if(line.begin(), line.end(),
                         [count=0](char c) mutable {
        switch(count) {
        case 0:
            count = c==':'?1:0;
            return false;
        case 1:
            count = c==' '?2:0;
            return true;
        default:
            return false;
        }
    });
    data.emplace(std::make_pair<std::string,std::string>(
            {line.begin(), iter-1},
            {iter+1, line.end()}));
}

request::filter request::match(const char * to_filer) {
    return {*this, to_filer};
}

void request::add_options(char *buffer, int size) {
    for(int i = 0, j = 0; i < size; i++) {
        if(buffer[i] == '\n') {
            buffer[i] = 0;
            match(buffer+j)
                .find("Host")
                .find("Accept")
                .find("Connection");
            j = i+1;
        }
    }
}

auto request::operator[](const std::string &arg) {
    return data[arg];
}

request::filter::filter(request& parent, const char *to_filer)
    : to_filer(to_filer)
    , parent(parent) {}

request::filter &request::filter::find(const char *pattern) {
    if(to_filer == nullptr) return *this;
    if(contains(pattern)) {
        parent.parse(to_filer);
        to_filer = nullptr;
    }
    return *this;
}

bool request::filter::contains(const char *pattern) {
    int i = 0;
    for(; pattern[i] != 0 && to_filer[i] != 0; i++) {
        if(to_filer[i] != pattern[i])
            return false;
    }
    return pattern[i] == 0;
}
