#pragma once

#include "connection_handler.h"

#include "httplib.h"

#include <iostream>
#include <json.hpp>
#include "command_handler.h"

using json = nlohmann::json;

namespace Sidequest
{
    namespace Server
    {
        std::function<void(const httplib::Request&, httplib::Response&)> CommandHandler::get_function()
        {
            auto function = std::bind(&CommandHandler::execute, this, std::placeholders::_1, std::placeholders::_2);
            return function;
        }
    } // namespace Server
} // namespace Sidequest