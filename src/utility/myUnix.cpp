#include "../include/myUnix.hpp"

namespace MyUnixFunc{

std::string myRead(std::shared_ptr<spdlog::logger> logger, int clientFD){
    if (!logger) {
        logger = spdlog::get("default_logger");
        if (!logger) {
            auto null_sink = std::make_shared<spdlog::sinks::null_sink_st>();
            logger = std::make_shared<spdlog::logger>("fallback", null_sink);
        }
    }

    char buffer[1024] ={0};
    std::string res;
    ssize_t bytesRead = 1;

    while(bytesRead > 0){
        bytesRead = read(clientFD, buffer, sizeof(buffer));

        if(bytesRead > 0){
            res.append(buffer, bytesRead);
            if(res.find("\r\n\r\n") != std::string::npos) break;
        }
        else if(bytesRead == 0)break;
        else throw std::runtime_error("read error");
    }

    logger->info("request received: " + res);
    return res;
}

ssize_t myWrite(std::shared_ptr<spdlog::logger> logger, int clientFD, std::string answer){
    if (!logger) {
        logger = spdlog::get("default_logger");
        if (!logger) {
            auto null_sink = std::make_shared<spdlog::sinks::null_sink_st>();
            logger = std::make_shared<spdlog::logger>("fallback", null_sink);
        }
    }
    
    ssize_t bytes = write(clientFD, answer.c_str(), answer.size());

    if(bytes < 0){
        logger->info("client {} disconected", clientFD);
        return bytes;
    }

    else if(bytes < answer.size()){
        logger->info("client {} received {} bytes", clientFD, bytes);
        return bytes;
    }

    else{
        logger->info("client received all of {} bytes", bytes);
        return bytes;
    }

}

}