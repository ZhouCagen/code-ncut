#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

class WebSocket
{
  public:
    enum class Opcode : std::uint8_t
    {
        Continuation = 0x0, // 后续分片帧，暂时可以不支持
        Text = 0x1,         // 文本消息，比如聊天室文字
        Binary = 0x2,       // 二进制消息，比如图片、文件
        Close = 0x8,        // 对方要关闭 WebSocket
        Ping = 0x9,         // 对方 ping 你
        Pong = 0xA          // 你回复 pong
    };

    struct Frame
    {
        bool fin{};
        Opcode opcode{};
        std::string payload;
    };

    WebSocket() = delete;

    static bool hasCompleteHandshakeRequest(const std::string &buffer);
    static std::string buildHandshakeResponse(const std::string &request);

    static std::optional<Frame> tryDecodeFrame(std::string &buffer);
    static std::string buildTextFrame(std::string_view text);
    static std::string buildBinaryFrame(std::string_view data);
    static std::string buildPongFrame(std::string_view payload);
    static std::string buildCloseFrame();

  private:
    static std::string getHeaderValue(const std::string &request,
                                      const std::string_view headerName);
    static std::string sha1Base64(const std::string &input);
    static std::string base64Encode(const unsigned char *data, std::size_t length);
};
