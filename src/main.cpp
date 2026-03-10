#include "../include/memory.hpp" 
#include "../include/sockets.hpp" 
#include "../include/myUnix.hpp"
#include "../include/dispatch.hpp"

int main(){
    std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("my_logger", "logs/log.txt");
    logger->flush_on(spdlog::level::info);

    int serverFD, clientFD;
    struct sockaddr_in addr;
    int port = 8080;

    struct sockaddr_in clientAddr;

    MySocketFunc::setSocket(logger, port, serverFD, addr);

    while(true){

        MySocketFunc::conectClient(logger, clientFD, clientAddr, serverFD);

        std::string request = MyUnixFunc::myRead(logger, clientFD);

        std::string answer = MyDispatchFunc::dispatch(logger, request);

        MyUnixFunc::myWrite(logger, clientFD, answer);

        close(clientFD);
    }
}