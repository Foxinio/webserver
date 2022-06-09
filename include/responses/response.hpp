//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <request.hpp>

#include <sstream>
#include <string>
#include <optional>
#include <memory>
#include <unistd.h>

class response {
protected:
    request& req;

public:
    response(request& req);
    virtual std::string get_header();
    virtual void fill_response(int outfd);
    void copy_file(int outfd);
protected:
    int standard_response_size();

    static const char *standard_response;
};

int write(int fd, const char *ptr);

class header_builder {
    std::string html_version = "HTTP/1.1";
    std::optional<std::string> date;
    std::optional<std::string> connection = "keep-alive";
    std::optional<std::string> server = "MyServer/0.1";
    std::optional<int> content_length;
    std::optional<std::string> content_type;
    std::optional<std::string> location;

public:
    enum content_type {
        other,
        txt,
        html,
        css,
        jpg,
        jpeg,
        png,
        pdf
    };

    std::stringstream to_string(const std::string &code) const;
    header_builder& with_html(const std::string&);
    header_builder& with_date(const std::string&);
    header_builder& with_current_date();
    header_builder& with_connection(const std::string&);
    header_builder& with_server(const std::string&);
    header_builder& with_content_length(int);
    header_builder& with_content_type(const std::string&);
    header_builder& with_content_type(enum content_type type);
    header_builder& with_location(const std::string&);

    static enum content_type parse_content_type(std::string file_path);
};
