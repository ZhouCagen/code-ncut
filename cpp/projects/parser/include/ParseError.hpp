#pragma once

#include "Token.hpp"

#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error
{
  public:
    ParseError(SourceLocation location, const std::string &message)
        : std::runtime_error("syntax error at line " + std::to_string(location.line) + ", column " +
                             std::to_string(location.column) + ": " + message),
          location_(location)
    {
    }

    SourceLocation location() const
    {
        return location_;
    }

  private:
    SourceLocation location_;
};
