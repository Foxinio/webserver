//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <responses/response.hpp>

class response_not_found : public response {
public:
    using response::response;
    std::string get_header() override;
    void fill_response(int outfd) override;
};
