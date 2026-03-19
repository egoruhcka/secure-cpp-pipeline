#include "../include/memory.hpp" 
#include "../include/sockets.hpp" 
#include "../include/myUnix.hpp"
#include "../include/dispatch.hpp"

int main(){
    std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt("my_logger", "../logs/log.txt");
    logger->flush_on(spdlog::level::info);
    std::signal(SIGPIPE, SIG_IGN);

    int serverFD = -1, clientFD = -1;
    struct sockaddr_in addr;
    int port = 8080;

    struct sockaddr_in clientAddr;
    try{
        MySocketFunc::setSocket(logger, port, serverFD, addr);}
    catch(const std::runtime_error& e){
        logger->error("cant set socket");
        return 1;
    }

    while(true){

        try{
            MySocketFunc::conectClient(logger, clientFD, clientAddr, serverFD);
        }catch(const std::exception& e){continue;}

        try{
            std::string request = MyUnixFunc::myRead(logger, clientFD);

            std::string answer = MyDispatchFunc::dispatch(logger, request);

            MyUnixFunc::myWrite(logger, clientFD, answer);
        }catch(const std::exception& e){
            logger->error("Worker error: {}", e.what());
            std::string errResp = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
            send(clientFD, errResp.c_str(), errResp.size(), MSG_NOSIGNAL);
        }

        if(clientFD > 0)close(clientFD);
    }
}