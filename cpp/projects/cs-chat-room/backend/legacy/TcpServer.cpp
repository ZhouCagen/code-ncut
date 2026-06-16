#include "TcpServer.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <thread>

namespace
{
    bool sendAllToClient(int clientSocket, const char* data, std::size_t length)
    {
        std::size_t totalSent = 0;
        while (totalSent < length)
        {
            ssize_t sendLen =
                send(clientSocket, data + totalSent, length - totalSent, MSG_NOSIGNAL);
            // 如果对面已经断开了，你服务器还继续send, Linux
            // 下可能会触发SIGPIPE，严重时程序直接被信号杀掉。
            if (sendLen <= 0)
                return false;

            totalSent += static_cast<std::size_t>(sendLen);
        }
        return true;
    }
} // namespace

TcpServer::TcpServer(std::uint16_t serverPort) : port_(serverPort), serverSocket_(-1)
{
}

TcpServer::~TcpServer()
{
    if (serverSocket_ != -1)
        close(serverSocket_);
}

void TcpServer::run()
{
    createSocket();
    setReuseAddress();
    bindAddress();
    listenSocket();
    acceptLoop();
}

void TcpServer::createSocket()
{
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ == -1)
    {
        spdlog::error("socket failed: {}", strerror(errno));
        throw std::runtime_error("socket failed");
    }

    spdlog::info("TCP server socket created successfully");
}

void TcpServer::setReuseAddress()
{
    int opt = 1;
    int setSockOptResult = setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opt,
                                      static_cast<socklen_t>(sizeof(opt)));
    if (setSockOptResult == -1)
    {
        spdlog::error("setsockopt failed: {}", strerror(errno));
        throw std::runtime_error("setsockopt failed");
    }
    spdlog::info("SO_REUSEADDR set successfully");
}

void TcpServer::bindAddress()
{
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port_);

    int bindResult = bind(serverSocket_, reinterpret_cast<sockaddr*>(&serverAddr),
                          static_cast<socklen_t>(sizeof(serverAddr)));
    if (bindResult == -1)
    {
        spdlog::error("bind failed: {}", strerror(errno));
        throw std::runtime_error("bind failed");
    }
    spdlog::info("TCP server bound successfully on port {}", port_);
}

void TcpServer::listenSocket()
{
    int listenResult = listen(serverSocket_, SOMAXCONN);
    if (listenResult == -1)
    {
        spdlog::error("listen failed: {}", strerror(errno));
        throw std::runtime_error("listen failed");
    }
    spdlog::info("TCP server listened successfully");
}

void TcpServer::acceptLoop()
{
    while (true)
    {
        sockaddr_in clientAddr{};
        socklen_t clientLen = static_cast<socklen_t>(sizeof(clientAddr));
        int clientSocket =
            accept(serverSocket_, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);

        if (clientSocket == -1)
        {
            spdlog::error("accept failed: {}", strerror(errno));
            continue;
        }

        char clientIp[INET_ADDRSTRLEN]{};
        const char* ipResult = inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);

        if (ipResult == nullptr)
        {
            spdlog::error("inet_ntop failed: {}", strerror(errno));
            close(clientSocket);
            continue;
        }

        std::uint16_t clientPort = ntohs(clientAddr.sin_port);

        spdlog::info("client connected: {}:{}", clientIp, clientPort);

        std::string clientIpString(clientIp);
        std::thread clientThread([this, clientSocket, clientIpString, clientPort]()
                                 { handleClient(clientSocket, clientIpString, clientPort); });

        clientThread.detach();
    }
}

void TcpServer::handleClient(int clientSocket, std::string clientIp, uint16_t clientPort)
{
    constexpr std::size_t bufferSize = 1024;
    while (true)
    {
        char buffer[bufferSize]{};
        ssize_t recvLen = recv(clientSocket, buffer, bufferSize - 1, 0);
        if (recvLen == 0)
        {
            spdlog::error("client disconnected unexpectedly");
            break;
        }
        if (recvLen == -1)
        {
            spdlog::error("recv failed: {}", strerror(errno));
            break;
        }

        std::string message(buffer, static_cast<size_t>(recvLen));
        spdlog::info("client says: {}", message);

        std::string reply = "server received: ";
        reply += message;

        bool isSend = sendAllToClient(clientSocket, reply.c_str(), reply.size());
        if (!isSend)
        {
            spdlog::error("send failed: {}", strerror(errno));
            break;
        }
    }

    close(clientSocket);
}
