#include <gtest/gtest.h>
#include <fstream>
#include "../include/verParser.hpp"

void createTestFile(const std::string& name, const std::string& content) {
    std::ofstream out(name);
    out << content;
    out.close();
}

TEST(VersionParserTest, SuccessPath) {
    std::string path = "valid_cmake.txt";
    createTestFile(path, "project(MyProject VERSION 1.2.3)");
    
    std::string result = MyVersionParser::parseVersionInformation(nullptr, path);
    
    EXPECT_EQ(result, "1.2.3");
    std::remove(path.c_str());
}

TEST(VersionParserTest, FileNotFound) {
    EXPECT_THROW({
        MyVersionParser::parseVersionInformation(nullptr, "non_existent_file.txt");
    }, std::runtime_error);
}

TEST(VersionParserTest, VersionNotFoundInFile) {
    std::string path = "empty_cmake.txt";
    createTestFile(path, "project(EmptyProject)\n# No version here");
    
    std::string result = MyVersionParser::parseVersionInformation(nullptr, path);
    
    EXPECT_EQ(result, "not found");
    std::remove(path.c_str());
}
