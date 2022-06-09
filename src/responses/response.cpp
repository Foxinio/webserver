//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response.hpp>

#include <sstream>

std::string response::get_response() {
    return header_builder()
            .with_connection("keep-alive")
            .to_string("501 Not Implemented").str();
}

response::response(request &req)
    : req(req) {}

std::stringstream header_builder::to_string(const std::string &code) const {
    std::stringstream ss;
    ss << html_version << " " << code << "\r\n";
    if(date) ss << "Date: " << *date << "\r\n";
    if(server) ss << "Server: " << *server << "\r\n";
    ss << "Content-Length: " << content_length << "\r\n";
    ss << "Connection: " << connection << "\r\n";
    if(content_type) ss << "Content-Type: " << *content_type << "\r\n";

    ss << "\r\n";
    return ss;
}

header_builder &header_builder::with_html(const std::string & html) {
    html_version = html;
    return *this;
}

header_builder &header_builder::with_date(const std::string &date) {
    this->date = date;
    return *this;
}

header_builder &header_builder::with_current_date() {
    date = "Sun, 18 Oct 2012 10:36:20 GMT";
    return *this;
}

header_builder &header_builder::with_connection(const std::string & arg) {
    connection = arg;
    return *this;
}

header_builder &header_builder::with_server(const std::string & arg) {
    server = arg;
    return *this;
}

header_builder &header_builder::with_content_length(int arg) {
    content_length = arg;
    return *this;
}

header_builder &header_builder::with_content_type(const std::string &arg) {
    content_type = arg;
    return *this;
}

header_builder &header_builder::with_content_type(enum header_builder::content_type type) {
    switch(type) {
    case content_type::txt:
        return with_content_type("text/plain");
    case content_type::html:
        return with_content_type("text/html");
    case content_type::css:
        return with_content_type("text/css");
    case content_type::jpg:
        return with_content_type("image/jpg");
    case content_type::jpeg:
        return with_content_type("image/jpeg");
    case content_type::png:
        return with_content_type("image/png");
    case content_type::pdf:
        return with_content_type("application/pdf");
    default:
        return *this;
    }
}

enum header_builder::content_type header_builder::parse_content_type(std::string file_path) {
    auto dot = std::find(file_path.rbegin(), file_path.rend(), '.');
    if(dot != file_path.rend()) {
        std::string extension = std::string(file_path.rbegin(), dot);
        std::reverse(extension.begin(), extension.end());
        if(extension == "html")
            return header_builder::content_type::html;
        else if(extension == "css")
            return header_builder::content_type::css;
        else if(extension == "txt")
            return header_builder::content_type::txt;
        else if(extension == "jpg")
            return header_builder::content_type::jpg;
        else if(extension == "jpeg")
            return header_builder::content_type::jpeg;
        else if(extension == "png")
            return header_builder::content_type::png;
        else if(extension == "pdf")
            return header_builder::content_type::pdf;
    }
    return content_type::txt;
}
