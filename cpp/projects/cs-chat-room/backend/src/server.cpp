#include "AsyncTcpServer.hpp"
#include "ChatServer.hpp"
#include "Database.hpp"
#include "WebSocketServer.hpp"

#include <cstdint>
#include <exception>
#include <spdlog/spdlog.h>

int main()
{
    try
    {
        constexpr std::uint16_t port = 2006;

        Database database("chat.db");
        database.initializeSchema();

        AsyncTcpServer tcpServer(port);
        WebSocketServer webSocketServer(tcpServer);
        ChatServer chatServer(webSocketServer, database);

        spdlog::info("chat server starting...");
        tcpServer.start();
    }
    catch (const std::exception &error)
    {
        spdlog::error("server crashed: {}", error.what());
        return 1;
    }

    return 0;
}
