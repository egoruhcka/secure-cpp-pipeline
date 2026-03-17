#include "../include/sockets.hpp" 
namespace MySocketFunc{

void setSocket(std::shared_ptr<spdlog::logger> logger, int& port, int& serverFD, struct sockaddr_in& addr){
    if (!logger) {
        logger = spdlog::get("default_logger");
        if (!logger) {
            auto null_sink = std::make_shared<spdlog::sinks::null_sink_st>();
            logger = std::make_shared<spdlog::logger>("fallback", null_sink);
        }
    }
    
    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFD == -1){
        logger->error("socket isnt create");
        throw std::runtime_error("socket isnt create");
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int error = bind(serverFD, (struct sockaddr*)&addr, sizeof(addr));
    if(error < 0){
        logger->error("8080 bind error");
        throw std::runtime_error("8080 bind error");
    }
    listen(serverFD, 3);
}

void conectClient(std::shared_ptr<spdlog::logger> logger, int& clientFD,
                sockaddr_in& clientAddr, int& serverFD){
    if (!logger) {
        logger = spdlog::get("default_logger");
        if (!logger) {
            auto null_sink = std::make_shared<spdlog::sinks::null_sink_st>();
            logger = std::make_shared<spdlog::logger>("fallback", null_sink);
        }
    }
    
                    socklen_t addrLen = sizeof(clientAddr);
    char clientIP[INET_ADDRSTRLEN] = {0};


    clientFD = accept(serverFD, (struct sockaddr*)&clientAddr, &addrLen);
        if(clientFD < 0){
            logger->error("accept error");
            return;
        }

        auto clientError = inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        if(clientError == nullptr){
            logger->error("incorrect IP");
            return;
        }
        int ClientPort = ntohs(clientAddr.sin_port);

        logger->info("{}:{} is conected on {}FD", clientIP, ClientPort, clientFD);
}
}
