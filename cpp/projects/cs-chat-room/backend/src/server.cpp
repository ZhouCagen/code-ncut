#include "AsyncTcpServer.hpp"

#include <cstdint>
#include <exception>
#include <spdlog/spdlog.h>

int main()
{
    constexpr std::uint16_t serverPort = 2006;

    try
    {
        AsyncTcpServer server(serverPort);
        server.start();
    }
    catch (const std::exception& error)
    {
        spdlog::error("server crashed: {}", error.what());
        return 1;
    }

    return 0;
}
