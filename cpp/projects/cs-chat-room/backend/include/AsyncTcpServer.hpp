#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

class AsyncTcpServer
{
  public:
    explicit AsyncTcpServer(std::uint16_t port);
    ~AsyncTcpServer();

    AsyncTcpServer(const AsyncTcpServer &) = delete;
    AsyncTcpServer &operator=(const AsyncTcpServer &) = delete;

    void start();
    void eventLoop();

    void setClientConnectedCallback(std::function<void(int)> callback);
    void setClientDisconnectedCallback(std::function<void(int)> callback);
    void setInputBufferCallback(std::function<void(int, std::string &)> callback);

    void enqueueWrite(int clientSocket, const std::string &data);
    void closeClient(int clientSocket);

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

  private:
    std::uint16_t port_{};
    int listenSocket_{-1};
    int epollInstance_{-1};

    std::unordered_map<int, std::string> inputBuffers_;
    std::unordered_map<int, std::string> outputBuffers_;

    std::function<void(int)> clientConnectedCallback_;
    std::function<void(int)> clientDisconnectedCallback_;
    std::function<void(int, std::string &)> inputBufferCallback_;
};
