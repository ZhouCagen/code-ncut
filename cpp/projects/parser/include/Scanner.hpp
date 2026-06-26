#pragma once

#include "Token.hpp"

#include <string_view>
#include <vector>

class Scanner
{
  public:
    explicit Scanner(std::string_view source);

    std::vector<Token> scan();

  private:
    std::string_view source_;
};
