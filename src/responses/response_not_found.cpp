//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response_not_found.hpp>

const char* not_found_response = R"abc(<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html>
<head>
   <title>403 Not Found</title>
</head>
<body>
   <h1>Requested File not found</h1>
   <p>Server didn't find resource you requested</p>
</body>
</html>
)abc";

std::string response_not_found::get_header() {
    return header_builder()
            .with_connection("keep-alive")
            .with_content_type(header_builder::html)
            .with_content_length(std::char_traits<char>::length(not_found_response))
            .to_string("404 Not Found").str();
}

void response_not_found::fill_response(int outfd) {
    write(outfd, not_found_response);
}
