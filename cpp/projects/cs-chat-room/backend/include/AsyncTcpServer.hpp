#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

class AsyncTcpServer
{
public:
    explicit AsyncTcpServer(std::uint16_t port);
    ~AsyncTcpServer();

    AsyncTcpServer(const AsyncTcpServer&) = delete;
    AsyncTcpServer& operator=(const AsyncTcpServer&) = delete;

    void start();
    void eventLoop();

private:
    void createSocket();
    void setReuseAddress();
    void setNonBlocking(int socketFd);
    void bindAddress();
    void listenOnSocket();

    void createEpoll();
    void addToEpoll(int socketFd, std::uint32_t events);
    void modifyEpoll(int socketFd, std::uint32_t events);
    void removeFromEpoll(int socketFd);

    void handleAccept();
    void handleRead(int clientSocket);
    void handleWrite(int clientSocket);

    void processInputBuffer(int clientSocket);
    void onTcpMessage(int clientSocket, const std::string& message);

    void queueSend(int clientSocket, const std::string& data);
    void closeClient(int clientSocket);

private:
    std::uint16_t port_{};
    int listenSocket_{-1};
    int epollInstance_{-1};

    std::unordered_map<int, std::string> inputBuffers_;
    std::unordered_map<int, std::string> outputBuffers_;
};
