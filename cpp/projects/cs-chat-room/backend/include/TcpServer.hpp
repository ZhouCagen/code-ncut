#pragma once

#include <cstdint>

class TcpServer
{
public:
    explicit TcpServer(std::uint16_t port);
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    void run();

private:
    void createSocket();
    void setReuseAddress();
    void bindAddress();
    void listenSocket();
    void acceptLoop();
    void handleClient(int clientSocket);

private:
    std::uint16_t port_;
    int serverSocket_;
};
