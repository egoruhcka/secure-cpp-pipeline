#include <gtest/gtest.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/resource.h>
#include "../include/sockets.hpp"
#include <thread>

TEST(SocketFuncTest, SetSocketSuccess) {
    int port = 0;
    int fd = -1;
    struct sockaddr_in addr;
    
    MySocketFunc::setSocket(nullptr, port, fd, addr);
    
    EXPECT_GT(fd, 0);
    EXPECT_EQ(addr.sin_family, AF_INET);
    
    close(fd);
}

TEST(SocketFuncTest, SetSocketBindError) {
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = 0;
    bind(listener, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    socklen_t len = sizeof(serv_addr);
    getsockname(listener, (struct sockaddr*)&serv_addr, &len);
    int busyPort = ntohs(serv_addr.sin_port);

    int fd;
    struct sockaddr_in addr;
    EXPECT_THROW(MySocketFunc::setSocket(nullptr, busyPort, fd, addr), std::runtime_error);
    
    close(listener);
}


TEST(SocketFuncTest, ConnectClientSuccess) {
    int port = 0;
    int serverFD, clientFD;
    struct sockaddr_in serverAddr, clientAddr;

    MySocketFunc::setSocket(nullptr, port, serverFD, serverAddr);
    
    socklen_t len = sizeof(serverAddr);
    getsockname(serverFD, (struct sockaddr*)&serverAddr, &len);

    std::thread clientThread([&serverAddr]() {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        close(sock);
    });

    MySocketFunc::conectClient(nullptr, clientFD, clientAddr, serverFD);

    EXPECT_GT(clientFD, 0);
    
    clientThread.join();
    close(clientFD);
    close(serverFD);
}

TEST(SocketFuncTest, SocketCreationError) {
    struct rlimit old_lim;
    getrlimit(RLIMIT_NOFILE, &old_lim);

    struct rlimit new_lim = {3, old_lim.rlim_max};
    setrlimit(RLIMIT_NOFILE, &new_lim);

    int port = 8080, fd;
    struct sockaddr_in addr;
    
    EXPECT_THROW(MySocketFunc::setSocket(nullptr, port, fd, addr), std::runtime_error);

    setrlimit(RLIMIT_NOFILE, &old_lim);
}

TEST(SocketFuncTest, AcceptError) {
    int invalidFD = -1; 
    int clientFD;
    struct sockaddr_in clientAddr;
    
    MySocketFunc::conectClient(nullptr, clientFD, clientAddr, invalidFD);
    
    EXPECT_LT(clientFD, 0);
}



