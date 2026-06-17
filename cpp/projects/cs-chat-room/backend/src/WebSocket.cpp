#include "WebSocket.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <openssl/evp.h>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace
{
    constexpr std::string_view websocketMagicGuid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    std::string toLower(std::string_view value)
    {
        std::string result(value);
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

        return result;
    }

    std::string trim(std::string_view value)
    {
        std::size_t begin = 0;
        while (begin < value.size() && std::isspace(static_cast<unsigned char>(value[begin])))
            begin++;

        std::size_t end = value.size();

        while (end > begin && std::isspace(static_cast<unsigned char>(value[end - 1])))
            end--;

        return std::string(value.substr(begin, end - begin));
    }
} // namespace

bool WebSocket::hasCompleteHandshakeRequest(const std::string& buffer)
{
    return buffer.find("\r\n\r\n") != std::string::npos;
}

std::string WebSocket::buildHandshakeResponse(const std::string& request)
{
    std::string secWebSocketKey = getHeaderValue(request, "Sec-WebSocket-Key");

    if (secWebSocketKey.empty())
        throw std::runtime_error("missing Sec-WebSocket-Key");

    std::string acceptInput(secWebSocketKey);
    acceptInput += websocketMagicGuid;

    std::string acceptKey = sha1Base64(acceptInput);

    std::string response;
    response += "HTTP/1.1 101 Switching Protocols\r\n";
    response += "Upgrade: websocket\r\n";
    response += "Connection: Upgrade\r\n";
    response += "Sec-WebSocket-Accept: " + acceptKey + "\r\n";
    response += "\r\n";

    return response;
}

std::string WebSocket::getHeaderValue(const std::string& request, const std::string_view headerName)
{
    /*
    std::string target;
    target.reserve(headerName.size() + 1);
    target.append(headerName);
    target.push_back(':');

    std::size_t headerStart = request.find(target);
    if (headerStart == std::string::npos)
        return {};

    std::size_t valueStart = headerStart + target.size();

    while (valueStart < request.size() &&
           (request[valueStart] == ' ' || request[valueStart] == '\t'))
        valueStart++;

    std::size_t valueEnd = request.find("\r\n", valueStart);

    if(valueEnd == std::string::npos)
        return {};

    return request.substr(valueStart, valueEnd - valueStart);
    */

    std::string targetName = toLower(headerName);
    std::size_t lineStart = 0;

    while (lineStart < request.size())
    {
        std::size_t lineEnd = request.find("\r\n", lineStart);
        if (lineEnd == std::string::npos)
            break;

        std::string_view line(request.data() + lineStart, lineEnd - lineStart);
        lineStart = lineEnd + 2;

        std::size_t colonPos = line.find(':');
        if (colonPos == std::string::npos)
            continue;

        std::string currentName = toLower(line.substr(0, colonPos));
        std::string currentValue = trim(line.substr(colonPos + 1));

        if (currentName == targetName)
            return currentValue;
    }

    return {};
}

std::string WebSocket::sha1Base64(const std::string& input)
{
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
    unsigned int hashLength = 0;

    int result =
        EVP_Digest(input.data(), input.size(), hash.data(), &hashLength, EVP_sha1(), nullptr);

    if (result != 1)
        throw std::runtime_error("SHA1 failed");

    return base64Encode(hash.data(), hashLength);
}

std::string WebSocket::base64Encode(const unsigned char* data, std::size_t length)
{
    if (length > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::runtime_error("base64 input too large");

    int inputLength = static_cast<int>(length);
    int outputLength = 4 * ((inputLength + 2) / 3);

    std::vector<unsigned char> output(outputLength + 1);
    int actualLength = EVP_EncodeBlock(output.data(), data, inputLength);

    if (actualLength < 0)
        throw std::runtime_error("base64 encode failed");

    return std::string(reinterpret_cast<char*>(output.data()),
                       static_cast<std::size_t>(actualLength));
}
