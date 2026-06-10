#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <spdlog/spdlog.h>

using namespace std;

int main()
{
    const char* serverIp = "127.0.0.1";
    const unsigned serverPort = 2006;
    const unsigned bufferSize = 1024;

    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1)
    {
        spdlog::error("socket failed: {}", strerror(errno));
        return EXIT_FAILURE;
    }

    spdlog::info("socket created successfully");
    sockaddr_in clientAddr{};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(serverPort);
    int ipResult = inet_pton(AF_INET, serverIp, &clientAddr.sin_addr);

    if (ipResult <= 0)
    {
        spdlog::error("inet_pton failed: {}", strerror(errno));
        close(clientSocket);
        return EXIT_FAILURE;
    }

    spdlog::info("UDP client started, server {}:{}", serverIp, serverPort);

    string message;
    char buffer[bufferSize];

    while (1)
    {
        ssize_t sendLen = sendto(clientSocket, message.c_str(), message.size(), 0,
                                 reinterpret_cast<sockaddr*>(&clientAddr),
                                 static_cast<socklen_t>(sizeof(clientAddr)));
        if (sendLen <= 0)
        {
            spdlog::error("sendto failed: {}", strerror(errno));
            continue;
        }
    }
}
