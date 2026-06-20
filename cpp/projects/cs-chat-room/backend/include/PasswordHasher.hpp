#pragma once

#include <string>
#include <string_view>

class PasswordHasher
{
  public:
    static std::string hashPassword(std::string_view password);
    static bool verifyPassword(std::string_view password, std::string_view storedHash);

  private:
    static std::string md5Hex(std::string_view input);
};
