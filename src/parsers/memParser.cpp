#include "../include/memory.hpp" 

namespace MyReportFunc{
const MemoryReport ParseMemoryInformation(std::shared_ptr<spdlog::logger> logger, std::istream* input) {
    if (!logger) {
        logger = spdlog::get("default_logger");
        if (!logger) {
            auto null_sink = std::make_shared<spdlog::sinks::null_sink_st>();
            logger = std::make_shared<spdlog::logger>("fallback", null_sink);
        }
    }

    std::ifstream file;
    std::istream* source = input;

    if (!source) {
        file.open("/proc/meminfo");
        if (!file.is_open()) {
            logger->error("file /proc/meminfo is not open");
            throw std::runtime_error("file by /proc/meminfo dont open");
        }
        source = &file; 
    }

    long t = 0, f = 0, a = 0;
    std::string line;
    
    while (std::getline(*source, line)) {
        std::stringstream ss(line);
        std::string name;
        long value;
        std::string unit;

        if (ss >> name >> value >> unit) {
            if (name == "MemTotal:") t = value;
            else if (name == "MemFree:") f = value;
            else if (name == "MemAvailable:") a = value;
        }
    }

    return MemoryReport(t, f, a);
}
}