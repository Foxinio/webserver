//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response_ok.hpp>

#include <responses/include_file.hpp>

std::string response_ok::get_response() {
    auto includer = include_file(req.requested_path);
    auto ss = header_builder()
            .with_connection("keep-alive")
            .with_content_type(header_builder::parse_content_type(req.requested_path))
            .with_content_length(includer.get_size())
            .to_string("200 OK");
    ss << includer;
    return ss.str();
}
