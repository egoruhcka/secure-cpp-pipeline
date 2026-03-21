#include "../include/memory.hpp" 
#include "../include/sockets.hpp" 
#include "../include/myUnix.hpp"
#include "../include/dispatch.hpp"
#include "../include/threadPool.hpp"

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

    ThreadPool pool(std::thread::hardware_concurrency(), logger);

    while(true){

        try{
            MySocketFunc::conectClient(logger, clientFD, clientAddr, serverFD);
        
            pool.enqueue(clientFD);
        }catch(const std::exception& e){continue;}
    }
}