#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <spdlog/spdlog.h>
#include <string>

using namespace std;

int main()
{
    constexpr uint16_t port = 2006;
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1)
    {
        spdlog::error("socket failed: {}", strerror(errno));
        return EXIT_FAILURE;
    }

    spdlog::info("UDP server socket created successfully");

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    int bindResult = bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr),
                          static_cast<socklen_t>(sizeof(serverAddr)));
    if (bindResult == -1)
    {
        spdlog::error("bind failed: {}", strerror(errno));
        close(serverSocket);
        return EXIT_FAILURE;
    }

    spdlog::info("UDP server bound successfully on port {}", port);

    constexpr size_t bufferSize = 1024;
    char buffer[bufferSize];

    while (true)
    {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = static_cast<socklen_t>(sizeof(clientAddr));
        ssize_t recvLen = recvfrom(serverSocket, buffer, bufferSize - 1, 0,
                                   reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
        if (recvLen == -1)
        {
            spdlog::error("recvfrom failed: {}", strerror(errno));
            continue;
        }

        buffer[recvLen] = '\0';
        char clientIp[INET_ADDRSTRLEN];

        const char* clientIpResult =
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, INET_ADDRSTRLEN);
        if (clientIpResult == nullptr)
        {
            spdlog::error("inet_ntop failed: {}", strerror(errno));
            continue;
        }

        uint16_t clientPort = ntohs(clientAddr.sin_port);

        spdlog::info("client[{}:{}] says {}", clientIp, clientPort, buffer);

        string reply = "server received: ";
        reply += buffer;
        ssize_t sendLen = sendto(serverSocket, reply.c_str(), reply.size(), 0,
                                 reinterpret_cast<sockaddr*>(&clientAddr), clientAddrLen);

        if (sendLen == -1)
        {
            spdlog::error("sendto failed: {}", strerror(errno));
            continue;
        }

        spdlog::info("reply sent, bytes: {}", sendLen);

        if (string(buffer) == "exit")
        {
            spdlog::info("server received exit, shutting down");
            break;
        }
    }

    close(serverSocket);

    return EXIT_SUCCESS;
}
