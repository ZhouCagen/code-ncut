#include "UserRepository.hpp"

#include <sqlite3.h>

#include <stdexcept>
#include <string>

namespace
{
std::runtime_error makeSqliteError(sqlite3 *database, const std::string &message)
{
    return std::runtime_error(message + ": " + sqlite3_errmsg(database));
}

std::string columnText(sqlite3_stmt *statement, int column)
{
    const unsigned char *text = sqlite3_column_text(statement, column);
    if (text == nullptr)
        return {};

    return reinterpret_cast<const char *>(text);
}

UserRecord readUser(sqlite3_stmt *statement)
{
    UserRecord user;
    user.id = sqlite3_column_int(statement, 0);
    user.account = columnText(statement, 1);
    user.nickname = columnText(statement, 2);
    user.passwordHash = columnText(statement, 3);
    user.phone = columnText(statement, 4);
    user.birthday = columnText(statement, 5);
    user.gender = columnText(statement, 6);
    user.email = columnText(statement, 7);

    return user;
}
} // namespace

UserRepository::UserRepository(Database &database) : database_(database)
{
}

bool UserRepository::createUser(std::string_view account, std::string_view nickname,
                                std::string_view passwordHash, std::string_view phone,
                                std::string_view birthday, std::string_view gender,
                                std::string_view email, int &createdUserId)
{
    sqlite3 *database = database_.raw();

    const char *sql = R"sql(
        INSERT INTO users (account, nickname, password_hash, phone, birthday, gender, email)
        VALUES (?, ?, ?, ?, ?, ?, ?);
    )sql";

    sqlite3_stmt *statement = nullptr;

    int prepareResult = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);
    if (prepareResult != SQLITE_OK)
        throw makeSqliteError(database, "prepare create user failed");

    sqlite3_bind_text(statement, 1, account.data(), static_cast<int>(account.size()),
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, nickname.data(), static_cast<int>(nickname.size()),
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 3, passwordHash.data(), static_cast<int>(passwordHash.size()),
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 4, phone.data(), static_cast<int>(phone.size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 5, birthday.data(), static_cast<int>(birthday.size()),
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 6, gender.data(), static_cast<int>(gender.size()),
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 7, email.data(), static_cast<int>(email.size()), SQLITE_TRANSIENT);

    int stepResult = sqlite3_step(statement);

    if (stepResult == SQLITE_CONSTRAINT)
    {
        sqlite3_finalize(statement);
        return false;
    }

    if (stepResult != SQLITE_DONE)
    {
        sqlite3_finalize(statement);
        throw makeSqliteError(database, "insert user failed");
    }

    createdUserId = static_cast<int>(sqlite3_last_insert_rowid(database));

    sqlite3_finalize(statement);
    return true;
}

std::optional<UserRecord> UserRepository::findByAccount(std::string_view account)
{
    sqlite3 *database = database_.raw();

    const char *sql = R"sql(
        SELECT id, account, nickname, password_hash, phone, birthday, gender, email
        FROM users
        WHERE account = ?;
    )sql";

    sqlite3_stmt *statement = nullptr;

    int prepareResult = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);
    if (prepareResult != SQLITE_OK)
        throw makeSqliteError(database, "prepare find user by account failed");

    sqlite3_bind_text(statement, 1, account.data(), static_cast<int>(account.size()),
                      SQLITE_TRANSIENT);

    int stepResult = sqlite3_step(statement);

    if (stepResult == SQLITE_ROW)
    {
        UserRecord user = readUser(statement);
        sqlite3_finalize(statement);
        return user;
    }

    if (stepResult == SQLITE_DONE)
    {
        sqlite3_finalize(statement);
        return std::nullopt;
    }

    sqlite3_finalize(statement);
    throw makeSqliteError(database, "find user by account failed");
}

std::optional<UserRecord> UserRepository::findById(int userId)
{
    sqlite3 *database = database_.raw();

    const char *sql = R"sql(
        SELECT id, account, nickname, password_hash, phone, birthday, gender, email
        FROM users
        WHERE id = ?;
    )sql";

    sqlite3_stmt *statement = nullptr;

    int prepareResult = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);
    if (prepareResult != SQLITE_OK)
        throw makeSqliteError(database, "prepare find user by id failed");

    sqlite3_bind_int(statement, 1, userId);

    int stepResult = sqlite3_step(statement);

    if (stepResult == SQLITE_ROW)
    {
        UserRecord user = readUser(statement);
        sqlite3_finalize(statement);
        return user;
    }

    if (stepResult == SQLITE_DONE)
    {
        sqlite3_finalize(statement);
        return std::nullopt;
    }

    sqlite3_finalize(statement);
    throw makeSqliteError(database, "find user by id failed");
}
