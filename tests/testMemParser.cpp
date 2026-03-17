#include <gtest/gtest.h>
#include <sstream>
#include "../include/memory.hpp"
#include <sys/resource.h>
#include <vector>
#include <fcntl.h>

TEST(MemoryParserTest, ParsesValidDataCorrectly) {
    std::stringstream mockInput;
    mockInput << "MemTotal:       1000 kB\n"
               << "MemFree:         500 kB\n"
               << "MemAvailable:    750 kB\n";

    MemoryReport report = MyReportFunc::ParseMemoryInformation(nullptr, &mockInput);

    EXPECT_EQ(report.getTotal(), 1000);
    EXPECT_EQ(report.getFree(), 500);
    EXPECT_EQ(report.getAvailable(), 750);
}

TEST(MemoryParserTest, HandlesEmptyInput) {
    std::stringstream emptyInput("");
    
    MemoryReport report = MyReportFunc::ParseMemoryInformation(nullptr, &emptyInput);

    EXPECT_EQ(report.getTotal(), 0);
    EXPECT_EQ(report.getFree(), 0);
    EXPECT_EQ(report.getAvailable(), 0);
}

TEST(MemoryParserTest, HandlesPartialData) {
    std::stringstream partialInput;
    partialInput << "MemTotal:       5000 kB\n\n"
                  << "SomeOtherField:  100 kB\n\n\n";

    MemoryReport report = MyReportFunc::ParseMemoryInformation(nullptr, &partialInput);

    EXPECT_EQ(report.getTotal(), 5000);
    EXPECT_EQ(report.getFree(), 0);
}

TEST(MemoryParserTest, ParsesValidDataCorrectlyFromProc) {
    MemoryReport report = MyReportFunc::ParseMemoryInformation(nullptr, nullptr);

    EXPECT_GE(report.getTotal(), 0);
    EXPECT_GE(report.getFree(), 0);
    EXPECT_GE(report.getAvailable(), 0);
}

TEST(MemoryParserTest, HandlesProcMeminfoError) {
    struct rlimit old_lim;
    getrlimit(RLIMIT_NOFILE, &old_lim);

    struct rlimit new_lim = {10, old_lim.rlim_max};
    setrlimit(RLIMIT_NOFILE, &new_lim);

    std::vector<int> fds;
    for(int i = 0; i < 20; ++i) {
        int fd = open("/dev/null", O_RDONLY);
        if (fd != -1) fds.push_back(fd);
    }

    EXPECT_THROW(MyReportFunc::ParseMemoryInformation(nullptr, nullptr), std::runtime_error);

    for(int fd : fds) close(fd);
    setrlimit(RLIMIT_NOFILE, &old_lim);
}
