#pragma once

#include "Database.hpp"
#include "UserRepository.hpp"
#include "WebSocketServer.hpp"

#include <nlohmann/json.hpp>

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct UserSession
{
    int userId{};
    std::string account;
    std::string nickname;
};

class ChatServer
{
  public:
    ChatServer(WebSocketServer &webSocketServer, Database &database);

    ChatServer(const ChatServer &) = delete;
    ChatServer &operator=(const ChatServer &) = delete;

  private:
    void handleClientConnected(int clientSocket);
    void handleClientDisconnected(int clientSocket);
    void handleTextMessage(int clientSocket, std::string_view text);

    void handleRegister(int clientSocket, const nlohmann::json &request);
    void handleLogin(int clientSocket, const nlohmann::json &request);
    void handlePrivateMessage(int clientSocket, const nlohmann::json &request);
    void handleCreateGroup(int clientSocket, const nlohmann::json &request);
    void handleJoinGroup(int clientSocket, const nlohmann::json &request);
    void handleGroupMessage(int clientSocket, const nlohmann::json &request);
    void handleWho(int clientSocket);

    std::optional<UserSession> getSession(int clientSocket) const;
    bool requireLogin(int clientSocket);

    void sendJson(int clientSocket, const nlohmann::json &response);
    void sendError(int clientSocket, std::string_view message);
    void sendSystemMessage(int clientSocket, std::string_view message);

    int createGroupInDatabase(std::string_view groupName, int ownerUserId);
    bool addGroupMember(int groupId, int userId, std::string_view role);
    bool isGroupMember(int groupId, int userId);
    std::vector<int> getGroupMemberUserIds(int groupId);
    void savePrivateMessage(int fromUserId, int toUserId, std::string_view content);
    void saveGroupMessage(int groupId, int fromUserId, std::string_view content);
    std::optional<UserRecord> findUserByAccount(std::string_view account);

  private:
    WebSocketServer &webSocketServer_;
    Database &database_;
    UserRepository userRepository_;

    std::unordered_map<int, UserSession> socketToSession_;
    std::unordered_map<int, int> userIdToSocket_;
};
