//
// Created by Szymon Jędras on 01/06/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <request.hpp>

request::request(char *buffer, int size) {
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

void request::parse(const std::string &line) {

}

request::filter request::match(const char * to_filer) {
    return {*this, to_filer};
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
