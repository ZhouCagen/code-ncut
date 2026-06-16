#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>

using namespace std;

bool sendAllToClient(int clientSocket, const char* data, size_t length)
{
    size_t totalSent = 0;
    while (totalSent < length)
    {
        ssize_t sendLen = send(clientSocket, data + totalSent, length - totalSent, MSG_NOSIGNAL);
        // 如果对面已经断开了，你服务器还继续send, Linux 下可能会触发SIGPIPE，严重时程序直接被信号杀掉。
        if (sendLen == -1)
            return false;

        totalSent += static_cast<size_t>(sendLen);
    }
    return true;
}

void handleClient(int clientSocket, string clientIp, uint16_t clientPort)
{
    constexpr size_t bufferSize = 1024;
    spdlog::info("client connected: {}:{}", clientIp, clientPort);

    while (true)
    {
        char buffer[bufferSize]{};
        ssize_t recvLen = recv(clientSocket, buffer, bufferSize - 1, 0);
        if (recvLen == 0)
        {
            spdlog::error("client disconnected unexpectedly: {}:{}", clientIp, clientPort);
            break;
        }

        if (recvLen == -1)
        {
            spdlog::error("recv failed from {}:{}: {}", clientIp, clientPort, strerror(errno));
            break;
        }

        string message(buffer, static_cast<size_t>(recvLen));
        spdlog::info("client[{}:{}] says {}", clientIp, clientPort, message);

        string reply = "server received: ";
        reply += message;

        bool isSend = sendAllToClient(clientSocket, reply.c_str(), reply.size());
        if (!isSend)
        {
            spdlog::error("send failed to {}:{}: {}", clientIp, clientPort, strerror(errno));
            break;
        }
    }

    close(clientSocket);
}

int main()
{
    constexpr uint16_t serverPort = 2006;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        spdlog::error("socket failed: {}", strerror(errno));
        return EXIT_FAILURE;
    }

    spdlog::info("TCP server socket created successfully");

    int opt = 1;
    // setsockopt 用来设置 socket 的选项。
    // 这里设置的是 SO_REUSEADDR，作用是允许服务器程序重启后，尽快重新绑定同一个 IP 和端口。
    // 如果不设置这个选项，服务器刚关闭后马上重新启动，bind() 可能会因为端口还处于 TIME_WAIT 等状态而失败，
    // 报错类似：Address already in use。

    // 参数说明：
    // serverSocket：要设置选项的 socket 文件描述符。
    // SOL_SOCKET：表示设置的是 socket 通用层级的选项。
    // SO_REUSEADDR：表示开启地址复用。
    // &opt：选项值的地址。opt = 1 表示开启，opt = 0 表示关闭。
    // sizeof(opt)：选项值占用的字节数，setsockopt 需要知道传入数据的大小。
    int setSockOptResult = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt,
                                      static_cast<socklen_t>(sizeof(opt)));
    if (setSockOptResult == -1)
    {
        spdlog::error("setsockopt failed: {}", strerror(errno));
        close(serverSocket);
        return EXIT_FAILURE;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);

    int bindResult = bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr),
                          static_cast<socklen_t>(sizeof(serverAddr)));
    if (bindResult == -1)
    {
        spdlog::error("bind failed: {}", strerror(errno));
        close(serverSocket);
        return EXIT_FAILURE;
    }

    spdlog::info("TCP server bound successfully on port {}", serverPort);

    int listenResult = listen(serverSocket, SOMAXCONN);
    if (listenResult == -1)
    {
        spdlog::error("listen failed: {}", strerror(errno));
        close(serverSocket);
        return EXIT_FAILURE;
    }

    spdlog::info("TCP server listened successfully");

    while (true)
    {
        sockaddr_in clientAddr{};
        socklen_t clientLen = static_cast<socklen_t>(sizeof(clientAddr));
        int clientSocket =
            accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);

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

        uint16_t clientPort = ntohs(clientAddr.sin_port);

        thread clientThread(handleClient, clientSocket, string(clientIp), clientPort);

        clientThread.detach();
    }

    close(serverSocket);
    return EXIT_SUCCESS;
}
