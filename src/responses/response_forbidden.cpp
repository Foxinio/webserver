//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response_forbidden.hpp>

const char* forbidden_response = R"abc(<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
<html>
<head>
   <title>403 Forbidden</title>
</head>
<body>
   <h1>Permission Denied</h1>
   <p>You requested access to resource you have no permission to.</p>
</body>
</html>
)abc";

std::string response_forbidden::get_header() {
    return header_builder()
            .with_connection(req["Connection-Type:"])
            .with_content_type(header_builder::html)
            .with_content_length(std::char_traits<char>::length(forbidden_response))
            .to_string("").str();
}

void response_forbidden::fill_response(int outfd) {
    write(outfd, forbidden_response);
}
