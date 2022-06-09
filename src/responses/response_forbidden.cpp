//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response_forbidden.hpp>

std::string response_forbidden::get_response() {
    return header_builder()
            .with_connection("keep-alive")
            .to_string("").str();
}
