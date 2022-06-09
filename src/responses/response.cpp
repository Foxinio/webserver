//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response.hpp>

#include <sys_wrapper.hpp>

#include <sstream>
#include <algorithm>
#include <iostream>
#include <fcntl.h>

const char* response::standard_response = R"abc(<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html>
<head>
   <title>400 Bad Request</title>
</head>
<body>
   <h1>Bad Request</h1>
   <p>Your browser sent a request that this server could not understand.</p>
   <p>The request line contained invalid characters following the protocol string.</p>
</body>
</html>
)abc";

std::string response::get_header() {
    return header_builder()
            .with_content_length(standard_response_size())
            .with_content_type(header_builder::html)
            .to_string("501 Not Implemented").str();
}

response::response(request &req)
    : req(req) {}

void response::fill_response(int outfd) {
    write(outfd, standard_response);
}

void response::copy_file(int outfd) {
    int filefd = open(req.requested_path.c_str(), O_RDONLY);
    if(filefd >= 0) {
        const int BUFFER_SIZE = 4096;
        char* buffer = new char[BUFFER_SIZE];
        while (true) {
            int red = read(filefd, buffer, BUFFER_SIZE);
            if(red <= 0)
                break;
            Write(outfd, buffer, red);
            std::string extension = {req.requested_path.begin() + req.requested_path.find_last_of('.') + 1, req.requested_path.end()};
            if(extension == "txt" || extension == "html")
                std::cout.write(buffer, red);
        }
        close(filefd);
    }
}

int response::standard_response_size() {
    return std::char_traits<char>::length(standard_response);
}


int write(int fd, const char *ptr) {
    return Write(fd, ptr, std::char_traits<char>::length(ptr));
}

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
        return with_content_type("text/plain; charset=utf-8");
    case content_type::html:
        return with_content_type("text/html; charset=utf-8");
    case content_type::css:
        return with_content_type("text/css; charset=utf-8");
    case content_type::jpg:
        return with_content_type("image/jpg");
    case content_type::jpeg:
        return with_content_type("image/jpeg");
    case content_type::png:
        return with_content_type("image/png");
    case content_type::pdf:
        return with_content_type("application/pdf");
    default:
        return with_content_type("application/octet-stream");
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
        else if(extension == "txt")
            return header_builder::content_type::txt;
    }
    return content_type::other;
}
