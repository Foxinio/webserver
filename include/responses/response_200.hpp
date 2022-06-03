//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <responses/response.hpp>

class response_200 : public response {
public:
    using response::response;
    std::string get_response() override;
};

class response_200_builder : public response_builder {
public:
    std::unique_ptr<response> filter(request &) override;
};
