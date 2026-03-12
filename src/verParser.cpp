#include "../include/verParser.hpp"

namespace MyVersionParser{

std::string parseVersionInformation(std::shared_ptr<spdlog::logger> logger){
    std::ifstream file("CMakeLists.txt");
    if(!file.is_open())throw std::runtime_error("cant find cmakefile");

    std::string line;

}

}