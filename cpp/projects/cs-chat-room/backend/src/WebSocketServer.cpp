#include "WebSocketServer.hpp"
#include "WebSocket.hpp"

#include <optional>
#include <spdlog/spdlog.h>
#include <utility>
#include <vector>

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
