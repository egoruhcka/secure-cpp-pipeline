#include "../include/verParser.hpp"

namespace MyVersionParser{

std::string parseVersionInformation(std::shared_ptr<spdlog::logger> logger){
    std::ifstream file("CMakeLists.txt");
    if(!file.is_open()){
        logger->error("CMakeFile is not found");
        throw std::runtime_error("cant find cmakefile");
    }

    std::string line;
    
    std::regex version(R"(project\s*\(.*VERSION\s+["']?([0-9.]+)["']?.*?\))", std::regex_constants::icase);
    std::smatch match;

    while(std::getline(file, line)){
        if(std::regex_search(line, match, version)){
            return match[1];
        }
    }

    logger->error("version is not found");
    return "not found";
}

}