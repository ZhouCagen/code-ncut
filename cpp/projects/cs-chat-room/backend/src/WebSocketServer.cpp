#include "WebSocketServer.hpp"
#include "WebSocket.hpp"

#include <optional>
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>

namespace
{
std::string toLowerCopy(std::string_view value)
{
    std::string result(value);
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

    return result;
}

bool isWebSocketUpgradeRequest(std::string_view request)
{
    std::string lowerRequest = toLowerCopy(request);

    return lowerRequest.find("upgrade: websocket") != std::string::npos &&
           lowerRequest.find("sec-websocket-key:") != std::string::npos;
}

std::string buildServerDashboardHtml()
{
    return R"html(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>聊天室服务器控制台</title>
    <style>
        * {
            box-sizing: border-box;
        }

        body {
            margin: 0;
            min-height: 100vh;
            font-family: Arial, "Microsoft YaHei", sans-serif;
            background: linear-gradient(135deg, #1d2671, #c33764);
            color: #ffffff;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .panel {
            width: 760px;
            padding: 36px;
            border-radius: 22px;
            background: rgba(255, 255, 255, 0.13);
            box-shadow: 0 24px 80px rgba(0, 0, 0, 0.35);
            backdrop-filter: blur(16px);
        }

        h1 {
            margin: 0 0 12px;
            font-size: 34px;
        }

        .subtitle {
            margin-bottom: 28px;
            opacity: 0.85;
            font-size: 16px;
        }

        .grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 16px;
            margin-bottom: 26px;
        }

        .card {
            padding: 18px;
            border-radius: 16px;
            background: rgba(255, 255, 255, 0.16);
        }

        .label {
            font-size: 14px;
            opacity: 0.75;
            margin-bottom: 8px;
        }

        .value {
            font-size: 22px;
            font-weight: bold;
        }

        .status {
            display: inline-flex;
            align-items: center;
            gap: 10px;
            padding: 12px 16px;
            border-radius: 999px;
            background: rgba(0, 255, 150, 0.2);
            color: #b8ffd8;
            font-weight: bold;
        }

        .dot {
            width: 12px;
            height: 12px;
            border-radius: 999px;
            background: #38ff99;
            box-shadow: 0 0 16px #38ff99;
        }

        .desc {
            margin-top: 24px;
            line-height: 1.8;
            opacity: 0.9;
        }

        code {
            padding: 3px 7px;
            border-radius: 6px;
            background: rgba(0, 0, 0, 0.25);
        }
    </style>
</head>
<body>
    <div class="panel">
        <div class="status">
            <span class="dot"></span>
            服务器正在运行
        </div>

        <h1>聊天室服务器控制台</h1>
        <div class="subtitle">C++ / epoll / WebSocket / SQLite 聊天服务器</div>

        <div class="grid">
            <div class="card">
                <div class="label">监听端口</div>
                <div class="value">2006</div>
            </div>

            <div class="card">
                <div class="label">网络模型</div>
                <div class="value">epoll ET</div>
            </div>

            <div class="card">
                <div class="label">通信协议</div>
                <div class="value">WebSocket</div>
            </div>

            <div class="card">
                <div class="label">数据库</div>
                <div class="value">SQLite</div>
            </div>
        </div>

        <div class="desc">
            当前服务器支持用户注册、登录、私聊、群聊、在线用户查询等功能。
            普通浏览器访问 <code>http://服务器IP:2006/</code> 会显示该服务器界面；
            聊天客户端通过 <code>ws://服务器IP:2006/</code> 建立 WebSocket 连接。
        </div>
    </div>
</body>
</html>
)html";
}

std::string buildHttpDashboardResponse()
{
    std::string body = buildServerDashboardHtml();

    std::string response;
    response += "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html; charset=utf-8\r\n";
    response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += body;

    return response;
}
} // namespace

WebSocketServer::WebSocketServer(AsyncTcpServer &tcpServer) : tcpServer_(tcpServer)
{
    tcpServer_.setClientConnectedCallback([this](int clientSocket)
                                          { handleTcpConnected(clientSocket); });

    tcpServer_.setClientDisconnectedCallback([this](int clientSocket)
                                             { handleTcpDisconnected(clientSocket); });

    tcpServer_.setInputBufferCallback([this](int clientSocket, std::string &inputBuffer)
                                      { handleTcpInput(clientSocket, inputBuffer); });
}

void WebSocketServer::setClientConnectedCallback(std::function<void(int)> callback)
{
    clientConnectedCallback_ = std::move(callback);
}

void WebSocketServer::setClientDisconnectedCallback(std::function<void(int)> callback)
{
    clientDisconnectedCallback_ = std::move(callback);
}

void WebSocketServer::setTextCallback(std::function<void(int, std::string_view)> callback)
{
    textCallback_ = std::move(callback);
}

void WebSocketServer::handleTcpConnected(int clientSocket)
{
    clientStates_[clientSocket] = ClientState::HttpHandshake;
    spdlog::info("websocket client state created, socket {}", clientSocket);
}

void WebSocketServer::handleTcpDisconnected(int clientSocket)
{
    auto stateIterator = clientStates_.find(clientSocket);

    bool wasWebSocketConnected = stateIterator != clientStates_.end() &&
                                 stateIterator->second == ClientState::WebSocketConnected;

    if (wasWebSocketConnected && clientDisconnectedCallback_)
        clientDisconnectedCallback_(clientSocket);

    clientStates_.erase(clientSocket);

    spdlog::info("websocket client state removed, socket {}", clientSocket);
}

void WebSocketServer::handleTcpInput(int clientSocket, std::string &inputBuffer)
{
    auto stateIterator = clientStates_.find(clientSocket);
    if (stateIterator == clientStates_.end())
    {
        spdlog::warn("client state not found for socket {}", clientSocket);
        return;
    }

    if (stateIterator->second == ClientState::HttpHandshake)
    {
        if (!WebSocket::hasCompleteHandshakeRequest(inputBuffer))
            return;

        std::size_t headerEnd = inputBuffer.find("\r\n\r\n");
        if (headerEnd == std::string::npos)
            return;

        std::string request = inputBuffer.substr(0, headerEnd + 4);

        if (!isWebSocketUpgradeRequest(request))
        {
            std::string response = buildHttpDashboardResponse();

            inputBuffer.erase(0, headerEnd + 4);

            tcpServer_.enqueueWrite(clientSocket, response);

            spdlog::info("server dashboard page served, socket {}", clientSocket);

            return;
        }

        try
        {
            std::string response = WebSocket::buildHandshakeResponse(request);

            inputBuffer.erase(0, headerEnd + 4);
            stateIterator->second = ClientState::WebSocketConnected;

            tcpServer_.enqueueWrite(clientSocket, response);

            spdlog::info("websocket handshake success, socket {}", clientSocket);

            if (clientConnectedCallback_)
                clientConnectedCallback_(clientSocket);
        }
        catch (const std::exception &error)
        {
            spdlog::warn("websocket handshake failed, socket {}, error: {}", clientSocket,
                         error.what());
            tcpServer_.closeClient(clientSocket);
            return;
        }
    }

    if (stateIterator->second == ClientState::WebSocketConnected)
    {
        try
        {
            while (true)
            {
                std::optional<WebSocket::Frame> frame = WebSocket::tryDecodeFrame(inputBuffer);

                if (!frame.has_value())
                    break;

                if (frame->opcode == WebSocket::Opcode::Text)
                {
                    onWebSocketText(clientSocket, frame->payload);
                }
                else if (frame->opcode == WebSocket::Opcode::Ping)
                {
                    spdlog::info("websocket ping from socket {}", clientSocket);
                    tcpServer_.enqueueWrite(clientSocket,
                                            WebSocket::buildPongFrame(frame->payload));
                }
                else if (frame->opcode == WebSocket::Opcode::Close)
                {
                    spdlog::info("websocket close from socket {}", clientSocket);
                    tcpServer_.enqueueWrite(clientSocket, WebSocket::buildCloseFrame());
                    tcpServer_.closeClient(clientSocket);
                    return;
                }
                else if (frame->opcode == WebSocket::Opcode::Binary)
                {
                    spdlog::info("websocket binary frame from socket {}, size {}", clientSocket,
                                 frame->payload.size());
                }
            }
        }
        catch (const std::exception &error)
        {
            spdlog::warn("websocket frame decode failed, socket {}, error: {}", clientSocket,
                         error.what());
            tcpServer_.closeClient(clientSocket);
        }
    }
}

void WebSocketServer::onWebSocketText(int clientSocket, std::string_view text)
{
    spdlog::info("websocket text from socket {}: {}", clientSocket, text);

    if (textCallback_)
        textCallback_(clientSocket, text);
}

void WebSocketServer::sendTextFrame(int clientSocket, std::string_view text)
{
    tcpServer_.enqueueWrite(clientSocket, WebSocket::buildTextFrame(text));
}

void WebSocketServer::broadcastTextFrame(int senderSocket, std::string_view text)
{
    std::string frame = WebSocket::buildTextFrame(text);

    std::vector<int> targetSockets;

    for (const auto &[socketFd, state] : clientStates_)
    {
        if (socketFd == senderSocket)
            continue;

        if (state != ClientState::WebSocketConnected)
            continue;

        targetSockets.push_back(socketFd);
    }

    for (int socketFd : targetSockets)
        tcpServer_.enqueueWrite(socketFd, frame);
}
