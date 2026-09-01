#include "../include/dispatch.hpp"
#include "../include/verParser.hpp"

namespace MyDispatchFunc{

std::string dispatch(std::shared_ptr<spdlog::logger> logger, std::string request){
    if (!logger) {
        logger = spdlog::get("default_logger");
        if (!logger) {
            auto null_sink = std::make_shared<spdlog::sinks::null_sink_st>();
            logger = std::make_shared<spdlog::logger>("fallback", null_sink);
        }
    }
    
    std::string html;
    std::string contentType = "text/html"; 

    if(request.find("GET") != std::string::npos){
        std::string getRequest = request.substr(request.find("GET") + 4, request.find(" HTTP") - request.find("GET") - 4);
        logger->debug("find get request");

        if (getRequest == "/metrics") {
            contentType = "text/plain; version=0.0.4; charset=utf-8";
        }

        html = dispatchGet(logger, getRequest);
        logger->debug("Extracted path: '{}'", getRequest);
    }
    
    std::string answer = "HTTP/1.1 200 OK\r\n"
                "Content-Type: " + contentType + "\r\n"
                "Content-Length: " + std::to_string(html.size()) + "\r\n"
                "\r\n" + html;
    
    return answer;
}

std::string dispatchGet(std::shared_ptr<spdlog::logger> logger, std::string request){
    if(request == "/metrics"){
        MemoryReport report = MyReportFunc::ParseMemoryInformation(logger, nullptr);
        return report.toPrometheusString();
    }

    if(request == "/memMetrics"){
        MemoryReport report = MyReportFunc::ParseMemoryInformation(logger, nullptr);
        return "MemoryReport{" + report.getString() + "}\n";
    }

    if(request == "/health"){
        return "{status=\"ok\"}\n";
    }

    if(request == "/version"){
        #ifndef APP_VERSION
            #define APP_VERSION "unknown"
        #endif
        return "version{version=\"" + std::string(APP_VERSION) + "\"}\n";
    }

    return "";
}
}