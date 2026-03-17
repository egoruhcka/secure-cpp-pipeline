#pragma once
#include "../include/memory.hpp" 

#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

namespace MySocketFunc{
void setSocket(std::shared_ptr<spdlog::logger> logger, int& port, int& serverFD, struct sockaddr_in& addr);
void conectClient(std::shared_ptr<spdlog::logger> logger, int& clientFD, sockaddr_in& clientAddr, int& serverFD);
}