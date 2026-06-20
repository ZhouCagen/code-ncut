#pragma once

#include "Database.hpp"

#include <optional>
#include <string>
#include <string_view>

struct UserRecord
{
    int id{};
    std::string account;
    std::string nickname;
    std::string passwordHash;
    std::string phone;
    std::string birthday;
    std::string gender;
    std::string email;
};

class UserRepository
{
  public:
    explicit UserRepository(Database &database);

    UserRepository(const UserRepository &) = delete;
    UserRepository &operator=(const UserRepository &) = delete;

    bool createUser(std::string_view account, std::string_view nickname,
                    std::string_view passwordHash, std::string_view phone,
                    std::string_view birthday, std::string_view gender, std::string_view email,
                    int &createdUserId);

    std::optional<UserRecord> findByAccount(std::string_view account);
    std::optional<UserRecord> findById(int userId);

  private:
    Database &database_;
};
