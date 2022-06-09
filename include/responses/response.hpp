//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <request.hpp>

#include <sstream>
#include <optional>
#include <memory>

class response {
protected:
    request& req;

public:
    response(request& req);
    virtual std::string get_response();

};

class header_builder {
    std::string html_version = "HTML/1.1";
    std::optional<std::string> date;
    std::string connection = "Closed";
    std::optional<std::string> server = "MyServer/0.1";
    int content_length = 0;
    std::optional<std::string> content_type;

public:
    enum class content_type {
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

    static enum content_type parse_content_type(std::string file_path);
};
