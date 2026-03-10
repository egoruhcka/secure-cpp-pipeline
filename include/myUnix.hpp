#pragma once

#include "sockets.hpp"

namespace MyUnixFunc{
std::string myRead(std::shared_ptr<spdlog::logger> logger, int clientFD);
ssize_t myWrite(std::shared_ptr<spdlog::logger> logger, int clientFD, std::string answer);
}