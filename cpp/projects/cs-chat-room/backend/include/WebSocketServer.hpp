#pragma once

#include "AsyncTcpServer.hpp"

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>

enum class ClientState
{
    HttpHandshake,
    WebSocketConnected
};

class WebSocketServer
{
  public:
    explicit WebSocketServer(AsyncTcpServer &tcpServer);

    WebSocketServer(const WebSocketServer &) = delete;
    WebSocketServer &operator=(const WebSocketServer &) = delete;

    void setClientConnectedCallback(std::function<void(int)> callback);
    void setClientDisconnectedCallback(std::function<void(int)> callback);

    void setTextCallback(std::function<void(int, std::string_view)> callback);

    void handleTcpConnected(int clientSocket);
    void handleTcpDisconnected(int clientSocket);
    void handleTcpInput(int clientSocket, std::string &inputBuffer);

    void sendTextFrame(int clientSocket, std::string_view text);
    void broadcastTextFrame(int senderSocket, std::string_view text);

  private:
    void onWebSocketText(int clientSocket, std::string_view text);

  private:
    AsyncTcpServer &tcpServer_;

    std::unordered_map<int, ClientState> clientStates_;

    std::function<void(int)> clientConnectedCallback_;
    std::function<void(int)> clientDisconnectedCallback_;
    std::function<void(int, std::string_view)> textCallback_;
};
