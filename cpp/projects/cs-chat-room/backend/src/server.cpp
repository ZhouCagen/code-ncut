#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

int createListenSocket(uint16_t port)
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "socket failed: " << strerror(errno) << std::endl;
        return -1;
    }

    int enable = 1;
    int setResult = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    if (setResult == -1)
    {
        std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
        close(serverSocket);
        return -1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    int bindResult = bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    if (bindResult == -1)
    {
        std::cerr << "bind failed: " << strerror(errno) << std::endl;
        close(serverSocket);
        return -1;
    }

    int listenResult = listen(serverSocket, SOMAXCONN);
    if (listenResult == -1)
    {
        std::cerr << "listen failed: " << strerror(errno) << std::endl;
        close(serverSocket);
        return -1;
    }

    return serverSocket;
}

int main(int argc, char* argv[])
{
    uint16_t port = 2006;
    if (argc >= 2)
    {
        port = static_cast<uint16_t>(std::stoi(argv[1]));
    }

    int serverSocket = createListenSocket(port);
    if (serverSocket == -1)
    {
        return EXIT_FAILURE;
    }

    std::cout << "C++ socket server listening on port " << port << std::endl;

    while (true)
    {
        sockaddr_in clientAddr{};
        socklen_t clientAddrLen = sizeof(clientAddr);

        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1)
        {
            std::cerr << "accept failed: " << strerror(errno) << std::endl;
            continue;
        }

        char clientIp[INET_ADDRSTRLEN]{};
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
        uint16_t clientPort = ntohs(clientAddr.sin_port);

        std::cout << "client connected: " << clientIp << ":" << clientPort << std::endl;

        char buffer[4096]{};
        ssize_t recvLen = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (recvLen > 0)
        {
            std::cout << "----- client data begin -----" << std::endl;
            std::cout << buffer << std::endl;
            std::cout << "----- client data end -----" << std::endl;
        }

        std::string reply = "HTTP/1.1 400 Bad Request\r\nContent-Length: 25\r\n\r\nWebSocket not ready yet.\n";
        send(clientSocket, reply.c_str(), reply.size(), MSG_NOSIGNAL);

        close(clientSocket);
    }

    close(serverSocket);
    return EXIT_SUCCESS;
}
