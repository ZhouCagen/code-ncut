#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

using namespace std;

bool sendAllToServer(int clientSocket, const char* data, size_t length)
{
    size_t totalSent = 0;
    while (totalSent < length)
    {
        ssize_t sendLen = send(clientSocket, data + totalSent, length - totalSent, 0);
        if (sendLen == -1)
            return false;
        totalSent += static_cast<size_t>(sendLen);
    }

    return true;
}

int main()
{
    constexpr const char* serverIp = "127.0.0.1";
    constexpr uint16_t serverPort = 2006;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        spdlog::error("socket failed: {}", strerror(errno));
        return EXIT_FAILURE;
    }

    spdlog::info("TCP client socket created successfully");

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    int ipResult = inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);

    if (ipResult == 0)
    {
        spdlog::error("invalid IPv4 address format: {}", serverIp);
        close(clientSocket);
        return EXIT_FAILURE;
    }

    if (ipResult == -1)
    {
        spdlog::error("inet_pton failed: {}", strerror(errno));
        close(clientSocket);
        return EXIT_FAILURE;
    }

    int connectResult = connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr),
                                static_cast<socklen_t>(sizeof(serverAddr)));

    if (connectResult == -1)
    {
        spdlog::error("connect failed: {}", strerror(errno));
        close(clientSocket);
        return EXIT_FAILURE;
    }

    spdlog::info("connected to TCP server {}:{}", serverIp, serverPort);

    constexpr size_t bufferSize = 1024;

    while (true)
    {
        cout << "Please enter the content you want to send: ";

        char buffer[bufferSize]{};
        string message;
        if (!getline(cin, message))
        {
            spdlog::error("input closed, client exiting");
            break;
        }
        if (message.empty())
            continue;

        if (message == "exit")
        {
            spdlog::info("client exiting");
            break;
        }

        bool isSend = sendAllToServer(clientSocket, message.c_str(), message.size());

        if (!isSend)
        {
            spdlog::error("send failed to {}:{}", serverIp, serverPort);
            break;
        }
        spdlog::info("message sent");

        ssize_t recvLen = recv(clientSocket, buffer, bufferSize - 1, 0);
        if (recvLen == 0)
        {
            spdlog::error("server disconnected connection unexpectedly");
            break;
        }
        if (recvLen == -1)
        {
            spdlog::error("recv failed from {}:{}: {}", serverIp, serverPort, strerror(errno));
            continue;
        }

        buffer[recvLen] = '\0';

        spdlog::info("server replies: {}", buffer);
    }

    close(clientSocket);
    return EXIT_SUCCESS;
}
