#pragma once

#include <memory>
#include <spdlog/sinks/rotating_file_sink.h> 
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>
#include <string>
#include <spdlog/sinks/null_sink.h> 
#include "spdlog/sinks/basic_file_sink.h"
#include <iostream> 


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

    std::string getString()const{return "TotalMemory:\"" + std::to_string(MemTotal) 
        + "\", FreeMemory=\"" + std::to_string(MemFree) + "\", AvailableMemory:" + std::to_string(MemAvailable);}

    std::string toPrometheusString() const {
        std::string metrics;
        
        long totalBytes = MemTotal * 1024;
        long freeBytes = MemFree * 1024;
        long availableBytes = MemAvailable * 1024;

        metrics += "# HELP node_memory_MemTotal_bytes Total memory size in bytes.\n";
        metrics += "# TYPE node_memory_MemTotal_bytes gauge\n";
        metrics += "node_memory_MemTotal_bytes " + std::to_string(totalBytes) + "\n";
        
        metrics += "# HELP node_memory_MemFree_bytes Free memory size in bytes.\n";
        metrics += "# TYPE node_memory_MemFree_bytes gauge\n";
        metrics += "node_memory_MemFree_bytes " + std::to_string(freeBytes) + "\n";
        
        metrics += "# HELP node_memory_MemAvailable_bytes Available memory size in bytes.\n";
        metrics += "# TYPE node_memory_MemAvailable_bytes gauge\n";
        metrics += "node_memory_MemAvailable_bytes " + std::to_string(availableBytes) + "\n";
        
        return metrics;
    }
};
namespace MyReportFunc{
const MemoryReport ParseMemoryInformation(
    std::shared_ptr<spdlog::logger> logger, 
    std::istream* input = nullptr
);
}
