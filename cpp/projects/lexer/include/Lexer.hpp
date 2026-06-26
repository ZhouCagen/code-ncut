#pragma once

#include "SymbolTable.hpp"
#include "Token.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

class Lexer
{
  public:
    explicit Lexer(SymbolTable &symbolTable);

    std::vector<Token> scan(std::string_view source);

    void writeTokens(const std::vector<Token> &tokens, const std::string &path) const;

  private:
    enum class State
    {
        Identifier,
        Number,
        OperatorOrPunctuation
    };

    std::runtime_error makeError(const SourceLocation &location, const std::string &message) const;

    SymbolTable &symbolTable_;
};
