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

const MemoryReport ParseMemoryInformation(std::shared_ptr<spdlog::logger> logger){
    if (!logger) {
        logger = spdlog::get("default_logger"); 
        if (!logger) logger = spdlog::null_logger_mt("null_logger");
    }
    
    logger->debug("start ParseMemoryInformation");

    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        logger->error("file is not open");
        throw std::runtime_error("file by /proc/meminfo dont open");
    }

    long t = 0, f = 0, a = 0;

    for(std::string line; std::getline(file, line); ){

        std::stringstream ss(line);
        std::string name;
        long value;
        std::string unit;

        if(ss >> name >> value >> unit){
            if(name == "MemTotal:"){
                t = value;
                logger->debug("MemTotal found: {} {}", value, unit);
            }else if(name == "MemFree:"){
                f = value;
                logger->debug("MemFree found: {} {}", value, unit);
            }else if(name == "MemAvailable:"){
                a = value;
                logger->debug("MemAvailable found: {} {}", value, unit);
            }
        }else{
            logger->debug("the line does not meet the format");
            continue;
        }
    }
    
    logger->debug("function completed successfully");

    return MemoryReport(t, f, a);
}