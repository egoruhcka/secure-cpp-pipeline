    #pragma once
    #include "sockets.hpp"

namespace MyDispatchFunc{
std::string dispatch(std::shared_ptr<spdlog::logger> logger, std::string request);
std::string dispatchGet(std::shared_ptr<spdlog::logger> logger, std::string request);
}