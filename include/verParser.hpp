#pragma once
#include "memory.hpp"

namespace MyVersionParser{
std::string parseVersionInformation(std::shared_ptr<spdlog::logger> logger);
}