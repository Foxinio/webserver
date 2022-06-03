//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <responses/response.hpp>

class response_501 : public response {
public:
    using response::response;
    std::string get_response() override;
};

class response_501_builder : public response_builder {
public:
    std::unique_ptr<response> filter(request &) override;
};
