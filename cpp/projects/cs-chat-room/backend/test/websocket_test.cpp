#include "WebSocket.hpp"

#include <iostream>
#include <string>

int main()
{
    std::string request = "GET /chat HTTP/1.1\r\n"
                          "Host: server.example.com\r\n"
                          "Upgrade: websocket\r\n"
                          "Connection: Upgrade\r\n"
                          "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
                          "Sec-WebSocket-Version: 13\r\n"
                          "\r\n";

    if (!WebSocket::hasCompleteHandshakeRequest(request))
    {
        std::cerr << "handshake request incomplete\n";
        return 1;
    }

    std::string response = WebSocket::buildHandshakeResponse(request);

    std::cout << response << '\n';

    const std::string expected = "Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";

    if (response.find(expected) == std::string::npos)
    {
        std::cerr << "accept key wrong\n";
        return 1;
    }

    std::cout << "websocket handshake test passed\n";
    return 0;
}
