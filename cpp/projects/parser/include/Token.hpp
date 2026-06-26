#pragma once

#include <cstddef>
#include <string>
#include <string_view>

enum class TokenType
{
    Identifier, // 标识符
    Constant,   // 常数
    Label,      // 标号

    If,   // IF
    Then, // THEN
    Else, // ELSE
    Goto, // GOTO

    Plus,  // +
    Minus, // -
    Star,  // *
    Slash, // /
    Power, // ^ 或 ↑

    Assign, // =

    Greater,      // >
    Less,         // <
    GreaterEqual, // >=
    LessEqual,    // <=
    NotEqual,     // <>

    LeftParen,  // (
    RightParen, // )
    Colon,      // :
    Semicolon,  // ;
    End         // #
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
};

inline std::string_view tokenTypeName(TokenType type)
{
    switch (type)
    {
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::Constant:
            return "Constant";
        case TokenType::Label:
            return "Label";
        case TokenType::If:
            return "IF";
        case TokenType::Then:
            return "THEN";
        case TokenType::Else:
            return "ELSE";
        case TokenType::Goto:
            return "GOTO";
        case TokenType::Plus:
            return "+";
        case TokenType::Minus:
            return "-";
        case TokenType::Star:
            return "*";
        case TokenType::Slash:
            return "/";
        case TokenType::Power:
            return "^";
        case TokenType::Assign:
            return "=";
        case TokenType::Greater:
            return ">";
        case TokenType::Less:
            return "<";
        case TokenType::GreaterEqual:
            return ">=";
        case TokenType::LessEqual:
            return "<=";
        case TokenType::NotEqual:
            return "<>";
        case TokenType::LeftParen:
            return "(";
        case TokenType::RightParen:
            return ")";
        case TokenType::Colon:
            return ":";
        case TokenType::Semicolon:
            return ";";
        case TokenType::End:
            return "#";
    }

    return "Unknown";
}

inline bool isRelationalOperator(TokenType type)
{
    return type == TokenType::Greater || type == TokenType::Less ||
           type == TokenType::GreaterEqual || type == TokenType::LessEqual ||
           type == TokenType::Assign || type == TokenType::NotEqual;
}
