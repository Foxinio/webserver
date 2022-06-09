//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#include <responses/response_moved_permanently.hpp>

std::string response_moved_permanently::get_response() {
    return header_builder()
            .with_connection("keep-alive")
            .to_string("301 Moved Permanently").str();
}
