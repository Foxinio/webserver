//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response_not_found.hpp>

std::string response_not_found::get_header() {
    return header_builder()
            .with_connection("keep-alive")
            .to_string("404 Not Found").str();
}
