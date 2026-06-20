#include "Database.hpp"

#include <sqlite3.h>

#include <stdexcept>
#include <string>

namespace
{
std::runtime_error makeSqliteError(sqlite3 *database, const std::string &message)
{
    return std::runtime_error(message + ": " + sqlite3_errmsg(database));
}
} // namespace

Database::Database(std::string databasePath)
{
    int openResult = sqlite3_open(databasePath.c_str(), &database_);
    if (openResult != SQLITE_OK)
    {
        std::string errorMessage = database_ ? sqlite3_errmsg(database_) : "unknown sqlite error";

        if (database_)
            sqlite3_close(database_);

        database_ = nullptr;

        throw std::runtime_error("sqlite open failed: " + errorMessage);
    }

    execute("PRAGMA foreign_keys = ON;");
}

Database::~Database()
{
    if (database_ != nullptr)
        sqlite3_close(database_);
}

sqlite3 *Database::raw()
{
    return database_;
}

void Database::execute(std::string_view sql)
{
    std::string sqlText(sql);

    char *errorMessage = nullptr;
    int execResult = sqlite3_exec(database_, sqlText.c_str(), nullptr, nullptr, &errorMessage);

    if (execResult != SQLITE_OK)
    {
        std::string message = errorMessage ? errorMessage : "unknown sqlite exec error";
        sqlite3_free(errorMessage);

        throw std::runtime_error("sqlite exec failed: " + message);
    }
}

void Database::initializeSchema()
{
    execute(R"sql(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            account TEXT NOT NULL UNIQUE,
            nickname TEXT NOT NULL,
            password_hash TEXT NOT NULL,
            phone TEXT NOT NULL UNIQUE,
            birthday TEXT NOT NULL,
            gender TEXT DEFAULT '',
            email TEXT DEFAULT '',
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
        );

        CREATE TABLE IF NOT EXISTS chat_groups (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            owner_user_id INTEGER NOT NULL,
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (owner_user_id) REFERENCES users(id) ON DELETE CASCADE
        );

        CREATE TABLE IF NOT EXISTS group_members (
            group_id INTEGER NOT NULL,
            user_id INTEGER NOT NULL,
            role TEXT NOT NULL DEFAULT 'member',
            joined_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
            PRIMARY KEY (group_id, user_id),
            FOREIGN KEY (group_id) REFERENCES chat_groups(id) ON DELETE CASCADE,
            FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
        );

        CREATE TABLE IF NOT EXISTS private_messages (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            from_user_id INTEGER NOT NULL,
            to_user_id INTEGER NOT NULL,
            content TEXT NOT NULL,
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (from_user_id) REFERENCES users(id) ON DELETE CASCADE,
            FOREIGN KEY (to_user_id) REFERENCES users(id) ON DELETE CASCADE
        );

        CREATE TABLE IF NOT EXISTS group_messages (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            group_id INTEGER NOT NULL,
            from_user_id INTEGER NOT NULL,
            content TEXT NOT NULL,
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (group_id) REFERENCES chat_groups(id) ON DELETE CASCADE,
            FOREIGN KEY (from_user_id) REFERENCES users(id) ON DELETE CASCADE
        );

        CREATE INDEX IF NOT EXISTS idx_users_account
            ON users(account);

        CREATE INDEX IF NOT EXISTS idx_users_phone
            ON users(phone);

        CREATE INDEX IF NOT EXISTS idx_private_messages_from_user_id
            ON private_messages(from_user_id);

        CREATE INDEX IF NOT EXISTS idx_private_messages_to_user_id
            ON private_messages(to_user_id);

        CREATE INDEX IF NOT EXISTS idx_group_members_user_id
            ON group_members(user_id);

        CREATE INDEX IF NOT EXISTS idx_group_messages_group_id
            ON group_messages(group_id);
    )sql");
}
