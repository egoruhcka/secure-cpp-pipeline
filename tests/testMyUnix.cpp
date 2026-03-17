#include <gtest/gtest.h>
#include <unistd.h>
#include "../include/myUnix.hpp"

TEST(MyUnixFuncTest, MyReadSuccess) {
    int pipefd[2];
    pipe(pipefd);

    std::string testData = "Hello\r\n\r\n";
    write(pipefd[1], testData.c_str(), testData.size());
    close(pipefd[1]);

    std::string result = MyUnixFunc::myRead(nullptr, pipefd[0]);
    
    EXPECT_EQ(result, "Hello\r\n\r\n");
    close(pipefd[0]);
}

TEST(MyUnixFuncTest, MyReadError) {
    EXPECT_THROW(MyUnixFunc::myRead(nullptr, -1), std::runtime_error);
}


TEST(MyUnixFuncTest, MyWriteFullSuccess) {
    int pipefd[2];
    pipe(pipefd);

    std::string data = "Response";
    ssize_t written = MyUnixFunc::myWrite(nullptr, pipefd[1], data);

    EXPECT_EQ(written, data.size());

    char buffer[10] = {0};
    read(pipefd[0], buffer, data.size());
    EXPECT_STREQ(buffer, "Response");

    close(pipefd[0]);
    close(pipefd[1]);
}

TEST(MyUnixFuncTest, MyWriteDisconnected) {
    int pipefd[2];
    pipe(pipefd);
    close(pipefd[1]);

    ssize_t result = MyUnixFunc::myWrite(nullptr, pipefd[1], "test");
    
    EXPECT_LT(result, 0);
    close(pipefd[0]);
}

#include <sys/socket.h>
#include <fcntl.h>

TEST(MyUnixFuncTest, MyWritePartialSuccess) {
    int sv[2];
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, sv), 0);

    int sndbuf = 1024;
    setsockopt(sv[0], SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf));

    fcntl(sv[0], F_SETFL, O_NONBLOCK);

    std::string bigData(100000, 'A');

    ssize_t written = MyUnixFunc::myWrite(nullptr, sv[0], bigData);

    EXPECT_GT(written, 0);
    EXPECT_LT(written, bigData.size());

    close(sv[0]);
    close(sv[1]);
}

