#include "../include/verParser.hpp"

namespace MyVersionParser{

std::string parseVersionInformation(std::shared_ptr<spdlog::logger> logger, const std::string& path){
    if (!logger) {
        logger = spdlog::get("default_logger");
        if (!logger) {
            auto null_sink = std::make_shared<spdlog::sinks::null_sink_st>();
            logger = std::make_shared<spdlog::logger>("fallback", null_sink);
        }
    }

    std::ifstream file(path);
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