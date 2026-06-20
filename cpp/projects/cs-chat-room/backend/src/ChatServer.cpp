#include "ChatServer.hpp"
#include "PasswordHasher.hpp"

#include <sqlite3.h>

#include <cctype>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace
{
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

std::string getStringField(const nlohmann::json &json, const char *key)
{
    if (!json.contains(key) || !json[key].is_string())
        return {};

    return json[key].get<std::string>();
}

int getIntField(const nlohmann::json &json, const char *key, int defaultValue = -1)
{
    if (!json.contains(key) || !json[key].is_number_integer())
        return defaultValue;

    return json[key].get<int>();
}

std::runtime_error makeSqliteError(sqlite3 *database, const std::string &message)
{
    return std::runtime_error(message + ": " + sqlite3_errmsg(database));
}
} // namespace

ChatServer::ChatServer(WebSocketServer &webSocketServer, Database &database)
    : webSocketServer_(webSocketServer), database_(database), userRepository_(database)
{
    webSocketServer_.setClientConnectedCallback([this](int clientSocket)
                                                { handleClientConnected(clientSocket); });

    webSocketServer_.setClientDisconnectedCallback([this](int clientSocket)
                                                   { handleClientDisconnected(clientSocket); });

    webSocketServer_.setTextCallback([this](int clientSocket, std::string_view text)
                                     { handleTextMessage(clientSocket, text); });
}

void ChatServer::handleClientConnected(int clientSocket)
{
    spdlog::info("chat websocket connected, socket {}", clientSocket);

    sendJson(clientSocket, {
                               {"type", "system"},
                               {"message", "已连接服务器，请先注册或登录"},
                           });
}

void ChatServer::handleClientDisconnected(int clientSocket)
{
    auto sessionIterator = socketToSession_.find(clientSocket);
    if (sessionIterator == socketToSession_.end())
        return;

    int userId = sessionIterator->second.userId;
    std::string account = sessionIterator->second.account;
    std::string nickname = sessionIterator->second.nickname;

    socketToSession_.erase(sessionIterator);

    auto socketIterator = userIdToSocket_.find(userId);
    if (socketIterator != userIdToSocket_.end() && socketIterator->second == clientSocket)
        userIdToSocket_.erase(socketIterator);

    spdlog::info("chat user disconnected, socket {}, account {}, nickname {}", clientSocket,
                 account, nickname);
}

void ChatServer::handleTextMessage(int clientSocket, std::string_view text)
{
    nlohmann::json request;

    try
    {
        request = nlohmann::json::parse(text);
    }
    catch (const std::exception &error)
    {
        sendError(clientSocket, "JSON 格式错误");
        return;
    }

    std::string type = getStringField(request, "type");

    if (type == "register")
    {
        handleRegister(clientSocket, request);
    }
    else if (type == "login")
    {
        handleLogin(clientSocket, request);
    }
    else if (type == "private_message")
    {
        handlePrivateMessage(clientSocket, request);
    }
    else if (type == "create_group")
    {
        handleCreateGroup(clientSocket, request);
    }
    else if (type == "join_group")
    {
        handleJoinGroup(clientSocket, request);
    }
    else if (type == "group_message")
    {
        handleGroupMessage(clientSocket, request);
    }
    else if (type == "who")
    {
        handleWho(clientSocket);
    }
    else
    {
        sendError(clientSocket, "未知消息类型");
    }
}

void ChatServer::handleRegister(int clientSocket, const nlohmann::json &request)
{
    std::string account = trim(getStringField(request, "account"));
    std::string nickname = trim(getStringField(request, "nickname"));
    std::string password = getStringField(request, "password");
    std::string phone = trim(getStringField(request, "phone"));
    std::string birthday = trim(getStringField(request, "birthday"));
    std::string gender = trim(getStringField(request, "gender"));
    std::string email = trim(getStringField(request, "email"));

    if (account.empty() || nickname.empty() || password.empty() || phone.empty() ||
        birthday.empty())
    {
        sendError(clientSocket, "账号、昵称、密码、手机号、出生日期不能为空");
        return;
    }

    if (account.size() > 32)
    {
        sendError(clientSocket, "账号太长，最多 32 个字符");
        return;
    }

    if (nickname.size() > 32)
    {
        sendError(clientSocket, "昵称太长，最多 32 个字符");
        return;
    }

    if (password.size() < 6)
    {
        sendError(clientSocket, "密码至少 6 位");
        return;
    }

    if (phone.size() < 6 || phone.size() > 20)
    {
        sendError(clientSocket, "手机号格式不正确");
        return;
    }

    std::string passwordHash = PasswordHasher::hashPassword(password);

    int createdUserId = -1;
    bool created = false;

    try
    {
        created = userRepository_.createUser(account, nickname, passwordHash, phone, birthday,
                                             gender, email, createdUserId);
    }
    catch (const std::exception &error)
    {
        spdlog::warn("register failed, error: {}", error.what());
        sendError(clientSocket, "注册失败，账号或手机号可能已存在");
        return;
    }

    if (!created)
    {
        sendJson(clientSocket, {
                                   {"type", "register_result"},
                                   {"success", false},
                                   {"message", "账号或手机号已存在"},
                               });
        return;
    }

    sendJson(clientSocket, {
                               {"type", "register_result"},
                               {"success", true},
                               {"message", "注册成功"},
                               {"user_id", createdUserId},
                               {"account", account},
                               {"nickname", nickname},
                           });
}

void ChatServer::handleLogin(int clientSocket, const nlohmann::json &request)
{
    std::string account = trim(getStringField(request, "account"));
    std::string password = getStringField(request, "password");

    if (account.empty() || password.empty())
    {
        sendError(clientSocket, "账号或密码不能为空");
        return;
    }

    auto user = userRepository_.findByAccount(account);
    if (!user.has_value())
    {
        sendJson(clientSocket, {
                                   {"type", "login_result"},
                                   {"success", false},
                                   {"message", "账号不存在"},
                               });
        return;
    }

    if (!PasswordHasher::verifyPassword(password, user->passwordHash))
    {
        sendJson(clientSocket, {
                                   {"type", "login_result"},
                                   {"success", false},
                                   {"message", "密码错误"},
                               });
        return;
    }

    socketToSession_[clientSocket] =
        UserSession{.userId = user->id, .account = user->account, .nickname = user->nickname};

    userIdToSocket_[user->id] = clientSocket;

    sendJson(clientSocket, {
                               {"type", "login_result"},
                               {"success", true},
                               {"message", "登录成功"},
                               {"user_id", user->id},
                               {"account", user->account},
                               {"nickname", user->nickname},

                               // 兼容旧前端字段，后面前端全改 nickname 后可以删
                               {"username", user->nickname},
                           });

    spdlog::info("user login success, socket {}, account {}, nickname {}", clientSocket,
                 user->account, user->nickname);
}

void ChatServer::handlePrivateMessage(int clientSocket, const nlohmann::json &request)
{
    if (!requireLogin(clientSocket))
        return;

    auto sender = getSession(clientSocket);
    if (!sender.has_value())
        return;

    std::string targetAccount = trim(getStringField(request, "to"));
    std::string content = trim(getStringField(request, "content"));

    if (targetAccount.empty() || content.empty())
    {
        sendError(clientSocket, "私聊目标账号或内容不能为空");
        return;
    }

    auto targetUser = userRepository_.findByAccount(targetAccount);
    if (!targetUser.has_value())
    {
        sendError(clientSocket, "目标账号不存在");
        return;
    }

    savePrivateMessage(sender->userId, targetUser->id, content);

    auto targetSocketIterator = userIdToSocket_.find(targetUser->id);
    if (targetSocketIterator == userIdToSocket_.end())
    {
        sendJson(clientSocket, {
                                   {"type", "private_message_result"},
                                   {"success", true},
                                   {"message", "消息已保存，但对方不在线"},
                                   {"to", targetAccount},
                                   {"to_nickname", targetUser->nickname},
                                   {"content", content},
                               });
        return;
    }

    int targetSocket = targetSocketIterator->second;

    sendJson(targetSocket, {
                               {"type", "private_message"},
                               {"from", sender->nickname},
                               {"from_account", sender->account},
                               {"content", content},
                           });

    sendJson(clientSocket, {
                               {"type", "private_message_result"},
                               {"success", true},
                               {"message", "发送成功"},
                               {"to", targetAccount},
                               {"to_nickname", targetUser->nickname},
                               {"content", content},
                           });
}

void ChatServer::handleCreateGroup(int clientSocket, const nlohmann::json &request)
{
    if (!requireLogin(clientSocket))
        return;

    auto session = getSession(clientSocket);
    if (!session.has_value())
        return;

    std::string groupName = trim(getStringField(request, "group_name"));
    if (groupName.empty())
    {
        sendError(clientSocket, "群聊名称不能为空");
        return;
    }

    int groupId = createGroupInDatabase(groupName, session->userId);
    addGroupMember(groupId, session->userId, "owner");

    sendJson(clientSocket, {
                               {"type", "create_group_result"},
                               {"success", true},
                               {"message", "创建群聊成功"},
                               {"group_id", groupId},
                               {"group_name", groupName},
                           });
}

void ChatServer::handleJoinGroup(int clientSocket, const nlohmann::json &request)
{
    if (!requireLogin(clientSocket))
        return;

    auto session = getSession(clientSocket);
    if (!session.has_value())
        return;

    int groupId = getIntField(request, "group_id");
    if (groupId <= 0)
    {
        sendError(clientSocket, "群号不合法");
        return;
    }

    bool added = addGroupMember(groupId, session->userId, "member");

    sendJson(clientSocket, {
                               {"type", "join_group_result"},
                               {"success", added},
                               {"message", added ? "加入群聊成功" : "你已经在群里，或者群聊不存在"},
                               {"group_id", groupId},
                           });
}

void ChatServer::handleGroupMessage(int clientSocket, const nlohmann::json &request)
{
    if (!requireLogin(clientSocket))
        return;

    auto sender = getSession(clientSocket);
    if (!sender.has_value())
        return;

    int groupId = getIntField(request, "group_id");
    std::string content = trim(getStringField(request, "content"));

    if (groupId <= 0 || content.empty())
    {
        sendError(clientSocket, "群号或消息内容不合法");
        return;
    }

    if (!isGroupMember(groupId, sender->userId))
    {
        sendError(clientSocket, "你不在这个群聊里");
        return;
    }

    saveGroupMessage(groupId, sender->userId, content);

    std::vector<int> memberUserIds = getGroupMemberUserIds(groupId);

    for (int userId : memberUserIds)
    {
        auto socketIterator = userIdToSocket_.find(userId);
        if (socketIterator == userIdToSocket_.end())
            continue;

        int targetSocket = socketIterator->second;

        sendJson(targetSocket, {
                                   {"type", "group_message"},
                                   {"group_id", groupId},
                                   {"from", sender->nickname},
                                   {"from_account", sender->account},
                                   {"content", content},
                               });
    }
}

void ChatServer::handleWho(int clientSocket)
{
    nlohmann::json users = nlohmann::json::array();

    for (const auto &[socketFd, session] : socketToSession_)
    {
        users.push_back({
            {"socket", socketFd},
            {"user_id", session.userId},
            {"account", session.account},
            {"nickname", session.nickname},

            // 兼容旧前端字段，后面前端全改 nickname 后可以删
            {"username", session.nickname},
        });
    }

    sendJson(clientSocket, {
                               {"type", "who_result"},
                               {"users", users},
                           });
}

std::optional<UserSession> ChatServer::getSession(int clientSocket) const
{
    auto iterator = socketToSession_.find(clientSocket);
    if (iterator == socketToSession_.end())
        return std::nullopt;

    return iterator->second;
}

bool ChatServer::requireLogin(int clientSocket)
{
    if (socketToSession_.find(clientSocket) != socketToSession_.end())
        return true;

    sendError(clientSocket, "请先登录");
    return false;
}

void ChatServer::sendJson(int clientSocket, const nlohmann::json &response)
{
    webSocketServer_.sendTextFrame(clientSocket, response.dump());
}

void ChatServer::sendError(int clientSocket, std::string_view message)
{
    sendJson(clientSocket, {
                               {"type", "error"},
                               {"message", std::string(message)},
                           });
}

void ChatServer::sendSystemMessage(int clientSocket, std::string_view message)
{
    sendJson(clientSocket, {
                               {"type", "system"},
                               {"message", std::string(message)},
                           });
}

int ChatServer::createGroupInDatabase(std::string_view groupName, int ownerUserId)
{
    sqlite3 *database = database_.raw();

    const char *sql = R"sql(
        INSERT INTO chat_groups (name, owner_user_id)
        VALUES (?, ?);
    )sql";

    sqlite3_stmt *statement = nullptr;
    int prepareResult = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if (prepareResult != SQLITE_OK)
        throw makeSqliteError(database, "prepare create group failed");

    sqlite3_bind_text(statement, 1, groupName.data(), static_cast<int>(groupName.size()),
                      SQLITE_TRANSIENT);
    sqlite3_bind_int(statement, 2, ownerUserId);

    int stepResult = sqlite3_step(statement);
    if (stepResult != SQLITE_DONE)
    {
        sqlite3_finalize(statement);
        throw makeSqliteError(database, "insert group failed");
    }

    sqlite3_finalize(statement);

    return static_cast<int>(sqlite3_last_insert_rowid(database));
}

bool ChatServer::addGroupMember(int groupId, int userId, std::string_view role)
{
    sqlite3 *database = database_.raw();

    const char *sql = R"sql(
        INSERT OR IGNORE INTO group_members (group_id, user_id, role)
        VALUES (?, ?, ?);
    )sql";

    sqlite3_stmt *statement = nullptr;
    int prepareResult = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if (prepareResult != SQLITE_OK)
        throw makeSqliteError(database, "prepare add group member failed");

    sqlite3_bind_int(statement, 1, groupId);
    sqlite3_bind_int(statement, 2, userId);
    sqlite3_bind_text(statement, 3, role.data(), static_cast<int>(role.size()), SQLITE_TRANSIENT);

    int stepResult = sqlite3_step(statement);
    if (stepResult != SQLITE_DONE)
    {
        sqlite3_finalize(statement);
        return false;
    }

    int changedRows = sqlite3_changes(database);

    sqlite3_finalize(statement);

    return changedRows > 0;
}

bool ChatServer::isGroupMember(int groupId, int userId)
{
    sqlite3 *database = database_.raw();

    const char *sql = R"sql(
        SELECT 1
        FROM group_members
        WHERE group_id = ? AND user_id = ?;
    )sql";

    sqlite3_stmt *statement = nullptr;
    int prepareResult = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if (prepareResult != SQLITE_OK)
        throw makeSqliteError(database, "prepare is group member failed");

    sqlite3_bind_int(statement, 1, groupId);
    sqlite3_bind_int(statement, 2, userId);

    int stepResult = sqlite3_step(statement);
    bool exists = stepResult == SQLITE_ROW;

    sqlite3_finalize(statement);

    return exists;
}

std::vector<int> ChatServer::getGroupMemberUserIds(int groupId)
{
    sqlite3 *database = database_.raw();

    const char *sql = R"sql(
        SELECT user_id
        FROM group_members
        WHERE group_id = ?;
    )sql";

    sqlite3_stmt *statement = nullptr;
    int prepareResult = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if (prepareResult != SQLITE_OK)
        throw makeSqliteError(database, "prepare get group members failed");

    sqlite3_bind_int(statement, 1, groupId);

    std::vector<int> userIds;

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        userIds.push_back(sqlite3_column_int(statement, 0));
    }

    sqlite3_finalize(statement);

    return userIds;
}

void ChatServer::savePrivateMessage(int fromUserId, int toUserId, std::string_view content)
{
    sqlite3 *database = database_.raw();

    const char *sql = R"sql(
        INSERT INTO private_messages (from_user_id, to_user_id, content)
        VALUES (?, ?, ?);
    )sql";

    sqlite3_stmt *statement = nullptr;
    int prepareResult = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if (prepareResult != SQLITE_OK)
        throw makeSqliteError(database, "prepare save private message failed");

    sqlite3_bind_int(statement, 1, fromUserId);
    sqlite3_bind_int(statement, 2, toUserId);
    sqlite3_bind_text(statement, 3, content.data(), static_cast<int>(content.size()),
                      SQLITE_TRANSIENT);

    int stepResult = sqlite3_step(statement);
    if (stepResult != SQLITE_DONE)
    {
        sqlite3_finalize(statement);
        throw makeSqliteError(database, "save private message failed");
    }

    sqlite3_finalize(statement);
}

void ChatServer::saveGroupMessage(int groupId, int fromUserId, std::string_view content)
{
    sqlite3 *database = database_.raw();

    const char *sql = R"sql(
        INSERT INTO group_messages (group_id, from_user_id, content)
        VALUES (?, ?, ?);
    )sql";

    sqlite3_stmt *statement = nullptr;
    int prepareResult = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if (prepareResult != SQLITE_OK)
        throw makeSqliteError(database, "prepare save group message failed");

    sqlite3_bind_int(statement, 1, groupId);
    sqlite3_bind_int(statement, 2, fromUserId);
    sqlite3_bind_text(statement, 3, content.data(), static_cast<int>(content.size()),
                      SQLITE_TRANSIENT);

    int stepResult = sqlite3_step(statement);
    if (stepResult != SQLITE_DONE)
    {
        sqlite3_finalize(statement);
        throw makeSqliteError(database, "save group message failed");
    }

    sqlite3_finalize(statement);
}

std::optional<UserRecord> ChatServer::findUserByAccount(std::string_view account)
{
    return userRepository_.findByAccount(account);
}
