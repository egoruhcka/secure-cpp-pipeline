#include "../include/threadPool.hpp"

void ThreadPool::workerLoop(std::shared_ptr<spdlog::logger> logger){
    while(!stop){
        sem.acquire();

        int clientFD = -1;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if(!discriptersQueue.empty()){
                clientFD = discriptersQueue.front();
                discriptersQueue.pop();
            }
        }

        if(clientFD != -1)
            handleClient(clientFD, logger);
    }
}

void ThreadPool::handleClient(int clientFD, std::shared_ptr<spdlog::logger> logger){
    try{
        std::string request = MyUnixFunc::myRead(logger, clientFD);

        std::string answer = MyDispatchFunc::dispatch(logger, request);

        MyUnixFunc::myWrite(logger, clientFD, answer);
    }catch(const std::exception& e){
        logger->error("Worker error: {}", e.what());
        std::string errResp = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
        send(clientFD, errResp.c_str(), errResp.size(), MSG_NOSIGNAL);
    }

    close(clientFD);
}

ThreadPool::ThreadPool(size_t threads, std::shared_ptr<spdlog::logger> logger){
    for(int i = 0; i < threads; ++i){
        workers.emplace_back(&ThreadPool::workerLoop, this, logger);
    }
}

void ThreadPool::enqueue(int clientFD){
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        discriptersQueue.push(clientFD);
    }

    sem.release();
}

ThreadPool::~ThreadPool(){
    stop = true;
    for(int i = 0; i < workers.size(); ++i)sem.release();
    for(auto& thread : workers)
        if(thread.joinable())thread.join();
}
