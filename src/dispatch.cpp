#include "../include/dispatch.hpp"

namespace MyDispatchFunc{

std::string dispatch(std::shared_ptr<spdlog::logger> logger, std::string request){
    std::string html;

    if(request.find("GET") != std::string::npos){
        std::string getRequest = request.substr(request.find("GET") + 4);
        logger->debug("find get request");
        std::string html = dispatchGet(logger, getRequest);
    }

    std::string answer = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + std::to_string(html.size()) + "\r\n"
                "\r\n" + html;
    
    return answer;
}

std::string dispatchGet(std::shared_ptr<spdlog::logger> logger, std::string request){
    if(request == "memMetrics"){
        MyReportFunc::MemoryReport report = MyReportFunc::ParseMemoryInformation(logger, nullptr);
        return "Memory report:\n" + report.getString();
    }

    if(request == "health"){
        return "{\"status\":\"ok\"}";
    }

    if(request == "version"){
        
    }

    return "";
}
}