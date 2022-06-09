//
// Created by Szymon Jędras on 6/9/22.
// Copyright (c) 2022 Szymon Jędras. All rights reserved.
//
#pragma once

#include <memory>
#include "responses/response.hpp"
#include <netinet/in.h>

bool is_forbidden(const std::string &path);
int Poll(int sockfd, int timeout);
std::unique_ptr<response> get_response(request& req);
void handle_connection(int accepted, sockaddr_in addr);
