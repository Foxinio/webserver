//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response_moved_permanently.hpp>

#include <responses/include_file.hpp>

std::string response_moved_permanently::get_header() {
    auto ss = header_builder()
            .with_location("http://" + req["Host"] + req.requested_path)
            .to_string("301 Moved Permanently");
    return ss.str();
}

void response_moved_permanently::fill_response(int outfd) {}
