#include "WebSocket.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <limits>
#include <openssl/evp.h>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <utility>
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

std::uint8_t getByte(std::string_view data, std::size_t index)
{
    return static_cast<std::uint8_t>(static_cast<unsigned char>(data[index]));
}

bool isKnownOpcode(WebSocket::Opcode opcode)
{
    switch (opcode)
    {
        case WebSocket::Opcode::Continuation:
        case WebSocket::Opcode::Text:
        case WebSocket::Opcode::Binary:
        case WebSocket::Opcode::Close:
        case WebSocket::Opcode::Ping:
        case WebSocket::Opcode::Pong:
            return true;
    }

    return false;
}

bool isControlOpcode(WebSocket::Opcode opcode)
{
    return opcode == WebSocket::Opcode::Close || opcode == WebSocket::Opcode::Ping ||
           opcode == WebSocket::Opcode::Pong;
}

std::string buildFrame(WebSocket::Opcode opcode, std::string_view payload)
{
    if (isControlOpcode(opcode) && payload.size() > 125)
        throw std::runtime_error("websocket control frame payload too large");

    std::string frame;

    std::uint8_t firstByte = 0x80 | static_cast<std::uint8_t>(opcode);
    frame.push_back(static_cast<char>(firstByte));

    std::uint64_t payloadLength = payload.size();

    if (payloadLength <= 125)
    {
        frame.push_back(static_cast<char>(0x00 | payloadLength));
    }
    else if (payloadLength <= 65535)
    {
        frame.push_back(static_cast<char>(0x00 | 126));
        frame.push_back(static_cast<char>((payloadLength >> 8) & 0xFF));
        frame.push_back(static_cast<char>(payloadLength & 0xFF));
    }
    else
    {
        frame.push_back(static_cast<char>(0x00 | 127));
        for (int i = 7; i >= 0; i--)
            frame.push_back(static_cast<char>((payloadLength >> (i * 8)) & 0xFF));
    }

    frame.append(payload);

    return frame;
}

} // namespace

bool WebSocket::hasCompleteHandshakeRequest(const std::string &buffer)
{
    return buffer.find("\r\n\r\n") != std::string::npos;
}

std::string WebSocket::buildHandshakeResponse(const std::string &request)
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

std::optional<WebSocket::Frame> WebSocket::tryDecodeFrame(std::string &buffer)
{
    // [第1字节 FIN(1)+RSV(3)+opcode(4)][第2字节MASK(1)+payload_len(7)][扩展长度，可选][mask_key，4字节][被 mask 的 payload]
    // FIN = 1：这条消息到这一帧就结束了 FIN = 0：后面还有分片 RSV 扩展位 普通 WebSocket 必须是：000
    // opcode 表示这一帧是什么类型
    // 浏览器 -> 服务器：MASK = 1 服务器 -> 浏览器：MASK = 0
    // payload_len 0 ~ 125 : 真实 payload
    // payload_len = 126 : 后面 2 字节才是真实长度
    // payload_len = 127 : 后面 8 字节才是真实长度

    if (buffer.size() < 2)
        return std::nullopt;
    std::uint8_t firstByte = getByte(buffer, 0);
    std::uint8_t secondByte = getByte(buffer, 1);

    bool fin = (firstByte & 0x80) != 0;
    bool hasReservedBits = (firstByte & 0x70) != 0;

    if (hasReservedBits)
        throw std::runtime_error("unsupported websocket reserved bits");

    Opcode opcode = static_cast<Opcode>(firstByte & 0x0F);

    if (!isKnownOpcode(opcode))
        throw std::runtime_error("unknown websocket opcode");

    bool masked = (secondByte & 0x80) != 0;
    std::uint64_t payloadLength = secondByte & 0x7F;

    std::size_t offset = 2;

    if (payloadLength == 126)
    {
        if (buffer.size() < offset + 2)
            return std::nullopt;

        payloadLength = 0;
        payloadLength = (static_cast<std::uint64_t>(getByte(buffer, offset)) << 8) |
                        static_cast<std::uint64_t>(getByte(buffer, offset + 1));

        offset += 2;
    }
    else if (payloadLength == 127)
    {
        if (buffer.size() < offset + 8)
            return std::nullopt;

        // 如果 8 字节长度的最高 bit 是 1，那这个 WebSocket frame 非法
        if ((getByte(buffer, offset) & 0x80) != 0)
            throw std::runtime_error("invalid websocket 64-bit payload length");

        payloadLength = 0;

        for (int i = 0; i < 8; ++i)
        {
            payloadLength <<= 8;
            payloadLength |= getByte(buffer, offset + i);
        }

        offset += 8;
    }

    if (payloadLength > static_cast<std::uint64_t>(std::numeric_limits<std::size_t>::max()))
        throw std::runtime_error("websocket payload too large");

    // WebSocket 协议规定控制帧有两个限制： 控制帧不能分片，所以 fin 必须是 true  控制帧 payload 长度不能超过 125
    if (isControlOpcode(opcode))
    {
        if (!fin)
            throw std::runtime_error("websocket control frame must not be fragmented");

        if (payloadLength > 125)
            throw std::runtime_error("websocket control frame payload too large");
    }
    else
    {
        if (!fin)
            throw std::runtime_error("fragmented websocket message is not supported");

        if (opcode == Opcode::Continuation)
            throw std::runtime_error("websocket continuation frame is not supported");
    }

    if (!masked)
        throw std::runtime_error("client websocket frame is not masked");

    std::array<std::uint8_t, 4> maskKey{};

    if (buffer.size() < offset + maskKey.size())
        return std::nullopt;

    for (std::size_t i = 0; i < maskKey.size(); ++i)
        maskKey[i] = getByte(buffer, offset + i);

    offset += maskKey.size();

    std::size_t realPayloadLength = static_cast<std::size_t>(payloadLength);

    if (realPayloadLength > buffer.size() - offset)
        return std::nullopt;

    std::string payload = buffer.substr(offset, realPayloadLength);

    for (std::size_t i = 0; i < payload.size(); ++i)
    {
        std::uint8_t decodedByte = getByte(payload, i) ^ maskKey[i % 4];
        payload[i] = static_cast<char>(decodedByte);
    }

    buffer.erase(0, offset + realPayloadLength);

    return Frame{.fin = fin, .opcode = opcode, .payload = std::move(payload)};
    // std::move(payload) 是把字符串资源移动进去，避免多拷贝一次
}

std::string WebSocket::buildTextFrame(std::string_view text)
{
    return buildFrame(Opcode::Text, text);
}

std::string WebSocket::buildBinaryFrame(std::string_view data)
{
    return buildFrame(Opcode::Binary, data);
}

std::string WebSocket::buildPongFrame(std::string_view pingPayload)
{
    return buildFrame(Opcode::Pong, pingPayload);
}

std::string WebSocket::buildCloseFrame()
{
    return buildFrame(Opcode::Close, {});
}

std::string WebSocket::getHeaderValue(const std::string &request, const std::string_view headerName)
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

std::string WebSocket::sha1Base64(const std::string &input)
{
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
    unsigned int hashLength = 0;

    int result =
        EVP_Digest(input.data(), input.size(), hash.data(), &hashLength, EVP_sha1(), nullptr);

    if (result != 1)
        throw std::runtime_error("SHA1 failed");

    return base64Encode(hash.data(), hashLength);
}

std::string WebSocket::base64Encode(const unsigned char *data, std::size_t length)
{
    if (length > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::runtime_error("base64 input too large");

    int inputLength = static_cast<int>(length);
    int outputLength = 4 * ((inputLength + 2) / 3);

    std::vector<unsigned char> output(outputLength + 1);
    int actualLength = EVP_EncodeBlock(output.data(), data, inputLength);

    if (actualLength < 0)
        throw std::runtime_error("base64 encode failed");

    return std::string(reinterpret_cast<char *>(output.data()),
                       static_cast<std::size_t>(actualLength));
}
