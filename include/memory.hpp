#pragma once

#include <memory>
#include <spdlog/sinks/rotating_file_sink.h> 
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>
#include <string>
#include <spdlog/sinks/null_sink.h> 

class MemoryReport{
private:
    long MemTotal = 0;
    long MemFree = 0;
    long MemAvailable = 0;
public:
    MemoryReport(long t, long f, long a): MemTotal(t), MemFree(f), MemAvailable(a) {}

    long getTotal()const{return MemTotal;}
    long getFree()const{return MemFree;}
    long getAvailable()const{return MemAvailable;}
};

const MemoryReport ParseMemoryInformation(std::shared_ptr<spdlog::logger> logger);
