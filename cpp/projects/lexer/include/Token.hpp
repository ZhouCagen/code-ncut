#pragma once

#include <cstddef>
#include <optional>
#include <string>

enum class TokenType
{
    Keyword,
    Identifier,
    Constant,
    Label,
    Operator,
    Punctuation,
    End
};

struct SourceLocation
{
    std::size_t line = 1;
    std::size_t column = 1;
};

struct Token
{
    TokenType type;
    std::string lexeme;
    SourceLocation location;
    std::optional<std::size_t> tableIndex;
};

inline char tokenTypeCode(TokenType type)
{
    switch (type)
    {
        case TokenType::Keyword:
            return 'K';
        case TokenType::Identifier:
            return 'I';
        case TokenType::Constant:
            return 'C';
        case TokenType::Label:
            return 'L';
        case TokenType::Operator:
            return 'O';
        case TokenType::Punctuation:
            return 'P';
        case TokenType::End:
            return '#';
    }

    return '?';
}

inline std::string_view tokenTypeName(TokenType type)
{
    switch (type)
    {
        case TokenType::Keyword:
            return "Keyword";
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::Constant:
            return "Constant";
        case TokenType::Label:
            return "Label";
        case TokenType::Operator:
            return "Operator";
        case TokenType::Punctuation:
            return "Punctuation";
        case TokenType::End:
            return "End";
    }

    return "Unknown";
}
