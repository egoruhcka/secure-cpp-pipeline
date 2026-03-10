#include "../include/myUnix.hpp"

namespace MyUnixFunc{

std::string myRead(std::shared_ptr<spdlog::logger> logger, int clientFD){
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

}