#include <gtest/gtest.h>
#include <sstream>
#include "../include/memory.hpp"

TEST(MemoryParserTest, ParsesValidDataCorrectly) {
    std::stringstream mockInput;
    mockInput << "MemTotal:       1000 kB\n"
               << "MemFree:         500 kB\n"
               << "MemAvailable:    750 kB\n";

    MemoryReport report = ParseMemoryInformation(nullptr, &mockInput);

    EXPECT_EQ(report.getTotal(), 1000);
    EXPECT_EQ(report.getFree(), 500);
    EXPECT_EQ(report.getAvailable(), 750);
}

TEST(MemoryParserTest, HandlesEmptyInput) {
    std::stringstream emptyInput("");
    
    MemoryReport report = ParseMemoryInformation(nullptr, &emptyInput);

    EXPECT_EQ(report.getTotal(), 0);
    EXPECT_EQ(report.getFree(), 0);
    EXPECT_EQ(report.getAvailable(), 0);
}

TEST(MemoryParserTest, HandlesPartialData) {
    std::stringstream partialInput;
    partialInput << "MemTotal:       5000 kB\n\n"
                  << "SomeOtherField:  100 kB\n\n\n";

    MemoryReport report = ParseMemoryInformation(nullptr, &partialInput);

    EXPECT_EQ(report.getTotal(), 5000);
    EXPECT_EQ(report.getFree(), 0);
}

