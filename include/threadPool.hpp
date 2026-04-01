#pragma once

#include "../include/sockets.hpp"
#include "../include/myUnix.hpp"
#include "../include/dispatch.hpp"
#include <atomic>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <semaphore>
#include <functional>

class ThreadPool{
private:
    std::vector<std::thread> workers;
    std::queue<int> discriptersQueue;
    std::mutex queueMutex;
    std::counting_semaphore<58> sem{0};
    std::atomic<bool> stop{false};

    void workerLoop(std::shared_ptr<spdlog::logger> logger);

    void handleClient(int clientFD, std::shared_ptr<spdlog::logger> logger);

public:
    ThreadPool(size_t threads, std::shared_ptr<spdlog::logger> logger);

    void enqueue(int clientFD);

    ~ThreadPool();
};