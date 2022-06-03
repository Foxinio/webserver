//
// Created by Szymon Jędras on 6/3/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <string>
#include <memory>

class request;

class response {
    request& req;

public:
    response(request& req);
    virtual std::string get_response();
};

class response_builder {
protected:
    std::unique_ptr<response_builder> next;
public:
    response_builder& with_next(std::unique_ptr<response_builder>&& next);
    virtual std::unique_ptr<response> filter(request&);
};
