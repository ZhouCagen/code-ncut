#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

using namespace std;

int main()
{
    constexpr const char* serverIp = "127.0.0.1";
    constexpr uint16_t serverPort = 2006;

    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1)
    {
        spdlog::error("socket failed: {}", strerror(errno));
        return EXIT_FAILURE;
    }

    spdlog::info("UDP client socket created successfully");
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);

    int ipResult = inet_pton(AF_INET, serverIp, &serverAddr.sin_addr);

    if (ipResult == 0)
    {
        spdlog::error("inet_pton failed ip: {}", serverIp);
        close(clientSocket);
        return EXIT_FAILURE;
    }
    if (ipResult == -1)
    {
        spdlog::error("inet_pton failed : {}", strerror(errno));
        close(clientSocket);
        return EXIT_FAILURE;
    }

    spdlog::info("UDP client started, server {}:{}", serverIp, serverPort);

    constexpr size_t bufferSize = 1024;
    char buffer[bufferSize];
    while (true)
    {
        cout << "Please enter the content you want to send: ";

        string message;
        if (!getline(cin, message))
        {
            spdlog::info("input closed, client exiting");
            break;
        }
        if (message.empty())
            continue;

        ssize_t sendLen = sendto(clientSocket, message.c_str(), message.size(), 0,
                                 reinterpret_cast<sockaddr*>(&serverAddr),
                                 static_cast<socklen_t>(sizeof(serverAddr)));
        if (sendLen == -1)
        {
            spdlog::error("sendto failed: {}", strerror(errno));
            continue;
        }

        spdlog::info("message sent, bytes: {}", sendLen);

        sockaddr_in fromAddr{};
        socklen_t fromAddrLen = static_cast<socklen_t>(sizeof(fromAddr));
        ssize_t recvLen = recvfrom(clientSocket, buffer, bufferSize - 1, 0,
                                   reinterpret_cast<sockaddr*>(&fromAddr), &fromAddrLen);

        if (recvLen == -1)
        {
            spdlog::error("recvfrom failed: {}", strerror(errno));
            continue;
        }

        if (fromAddr.sin_addr.s_addr != serverAddr.sin_addr.s_addr ||
            fromAddr.sin_port != serverAddr.sin_port)
        {
            spdlog::warn("received packet from unexpected address");
            continue;
        }

        buffer[recvLen] = '\0';

        char fromIp[INET_ADDRSTRLEN];

        const char* fromIpResult = inet_ntop(AF_INET, &fromAddr.sin_addr, fromIp, INET_ADDRSTRLEN);
        if (fromIpResult == nullptr)
        {
            spdlog::error("inet_ntop failed: {}", strerror(errno));
            continue;
        }

        uint16_t fromPort = ntohs(fromAddr.sin_port);

        spdlog::info("server[{}:{}] replies: {}", fromIp, fromPort, buffer);

        if (message == "exit")
        {
            spdlog::info("client exiting");
            break;
        }
    }

    close(clientSocket);
    return EXIT_SUCCESS;
}
