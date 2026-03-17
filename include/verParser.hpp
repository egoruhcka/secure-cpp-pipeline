#pragma once
#include "memory.hpp"
#include <regex>

namespace MyVersionParser{
std::string parseVersionInformation(std::shared_ptr<spdlog::logger> logger, const std::string& path);
}