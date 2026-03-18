#include <gtest/gtest.h>
#include "../include/dispatch.hpp"
#include <spdlog/sinks/null_sink.h>

TEST(DispatchSimpleTest, HandlesHealth) {
    std::string response = MyDispatchFunc::dispatch(nullptr, "GET /health HTTP");
    
    EXPECT_NE(response.find("{status=\"ok\"}"), std::string::npos);
    EXPECT_NE(response.find("Content-Length: 14"), std::string::npos);
}

TEST(DispatchSimpleTest, NoGetRequest) {
    std::string response = MyDispatchFunc::dispatch(nullptr, "POST /data");
    
    EXPECT_NE(response.find("Content-Length: 0"), std::string::npos);
}

TEST(DispatchSimpleTest, HandlesMemMetrics) {
    auto logger = std::make_shared<spdlog::logger>("test", std::make_shared<spdlog::sinks::null_sink_st>());
    std::string response = MyDispatchFunc::dispatch(logger, "GET /memMetrics HTTP");
    
    EXPECT_NE(response.find("MemoryReport{"), std::string::npos);
}

TEST(DispatchSimpleTest, UnknownPath) {
    std::string response = MyDispatchFunc::dispatch(nullptr, "GET unknown_page");
    
    EXPECT_NE(response.find("Content-Length: 0"), std::string::npos);
}

#include <gtest/gtest.h>
#include <regex>
#include "../include/dispatch.hpp"

TEST(DispatchSimpleTest, HandlesVersionFormat) {
    std::string response = MyDispatchFunc::dispatch(nullptr, "GET /version HTTP");

    ASSERT_NE(response.find("version{version=\""), std::string::npos);

    std::regex versionRegex(R"(\d+\.\d+(\.\d+)?)");
    std::smatch match;

    bool found = std::regex_search(response, match, versionRegex);

    EXPECT_TRUE(found) << "Версия в ответе '" << response << "' не соответствует формату Ч.С.Ч";
}


