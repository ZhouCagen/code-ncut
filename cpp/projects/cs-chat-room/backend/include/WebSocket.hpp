#pragma once

#include <string>

class WebSocket
{
public:
    WebSocket() = delete;

    static bool hasCompleteHandshakeRequest(const std::string& buffer);
    static std::string buildHandshakeResponse(const std::string& request);

private:
    static std::string getHeaderValue(const std::string& request,
                                      const std::string_view headerName);
    static std::string sha1Base64(const std::string& input);
    static std::string base64Encode(const unsigned char* data, std::size_t length);
};
