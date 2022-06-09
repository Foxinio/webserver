//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response_not_implemented.hpp>

#include <request.hpp>

response_not_implemented::response_not_implemented()
    : response(request::empty) {}

std::string response_not_implemented::get_response() {
    return header_builder()
            .with_connection("keep-alive")
            .to_string("501 Not Implemented").str();
}
