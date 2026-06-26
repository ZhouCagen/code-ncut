#include "Scanner.hpp"

#include <cctype>
#include <stdexcept>
#include <string>

namespace
{
enum class State
{
    Identifier,
    Number,
    OperatorOrPunctuation
};
} // namespace

Scanner::Scanner(std::string_view source) : source_(source)
{
}

std::vector<Token> Scanner::scan()
{
    std::vector<Token> tokens;

    std::size_t i = 0;
    std::size_t line = 1;
    std::size_t column = 1;

    bool previousTokenIsGoto = false;

    auto advance = [&]()
    {
        char ch = source_[i];
        i++;

        if (ch == '\n')
        {
            line++;
            column = 1;
        }
        else
        {
            column++;
        }

        return ch;
    };

    while (i < source_.size())
    {
        char ch = source_[i];

        if (std::isspace(static_cast<unsigned char>(ch)))
        {
            advance();
            continue;
        }

        SourceLocation location{.line = line, .column = column};

        if (ch == '#')
        {
            advance();

            tokens.push_back(Token{.type = TokenType::End, .lexeme = "#", .location = location});
            break;
        }

        State state;

        if (std::isalpha(static_cast<unsigned char>(ch)))
            state = State::Identifier;
        else if (std::isdigit(static_cast<unsigned char>(ch)))
            state = State::Number;
        else
            state = State::OperatorOrPunctuation;

        switch (state)
        {
            case State::Identifier:
            {
                std::string word;

                while (i < source_.size() &&
                       (std::isalnum(static_cast<unsigned char>(source_[i])) || source_[i] == '_'))
                {
                    word += advance();
                }

                if (word == "IF")
                {
                    tokens.push_back(
                        Token{.type = TokenType::If, .lexeme = word, .location = location});
                    previousTokenIsGoto = false;
                }
                else if (word == "THEN")
                {
                    tokens.push_back(
                        Token{.type = TokenType::Then, .lexeme = word, .location = location});
                    previousTokenIsGoto = false;
                }
                else if (word == "ELSE")
                {
                    tokens.push_back(
                        Token{.type = TokenType::Else, .lexeme = word, .location = location});
                    previousTokenIsGoto = false;
                }
                else if (word == "GOTO")
                {
                    tokens.push_back(
                        Token{.type = TokenType::Goto, .lexeme = word, .location = location});
                    previousTokenIsGoto = true;
                }
                else
                {
                    tokens.push_back(
                        Token{.type = TokenType::Identifier, .lexeme = word, .location = location});
                    previousTokenIsGoto = false;
                }

                break;
            }

            case State::Number:
            {
                std::string number;

                while (i < source_.size() && std::isdigit(static_cast<unsigned char>(source_[i])))
                {
                    number += advance();
                }

                if (i < source_.size() &&
                    (std::isalpha(static_cast<unsigned char>(source_[i])) || source_[i] == '_'))
                {
                    throw std::runtime_error("scanner error at line " + std::to_string(line) +
                                             ", column " + std::to_string(column) +
                                             ": number cannot be followed by identifier");
                }

                bool isLabel = previousTokenIsGoto || (i < source_.size() && source_[i] == ':');

                tokens.push_back(Token{
                    .type = isLabel ? TokenType::Label : TokenType::Constant,
                    .lexeme = number,
                    .location = location,
                });

                previousTokenIsGoto = false;
                break;
            }

            case State::OperatorOrPunctuation:
            {
                if (ch == '>')
                {
                    advance();

                    if (i < source_.size() && source_[i] == '=')
                    {
                        advance();

                        tokens.push_back(Token{
                            .type = TokenType::GreaterEqual,
                            .lexeme = ">=",
                            .location = location,
                        });
                    }
                    else
                    {
                        tokens.push_back(
                            Token{.type = TokenType::Greater, .lexeme = ">", .location = location});
                    }

                    previousTokenIsGoto = false;
                    break;
                }

                if (ch == '<')
                {
                    advance();

                    if (i < source_.size() && source_[i] == '=')
                    {
                        advance();

                        tokens.push_back(Token{
                            .type = TokenType::LessEqual,
                            .lexeme = "<=",
                            .location = location,
                        });
                    }
                    else if (i < source_.size() && source_[i] == '>')
                    {
                        advance();

                        tokens.push_back(Token{
                            .type = TokenType::NotEqual,
                            .lexeme = "<>",
                            .location = location,
                        });
                    }
                    else
                    {
                        tokens.push_back(
                            Token{.type = TokenType::Less, .lexeme = "<", .location = location});
                    }

                    previousTokenIsGoto = false;
                    break;
                }

                switch (ch)
                {
                    case '+':
                        advance();
                        tokens.push_back(
                            Token{.type = TokenType::Plus, .lexeme = "+", .location = location});
                        break;

                    case '-':
                        advance();
                        tokens.push_back(
                            Token{.type = TokenType::Minus, .lexeme = "-", .location = location});
                        break;

                    case '*':
                        advance();
                        tokens.push_back(
                            Token{.type = TokenType::Star, .lexeme = "*", .location = location});
                        break;

                    case '/':
                        advance();
                        tokens.push_back(
                            Token{.type = TokenType::Slash, .lexeme = "/", .location = location});
                        break;

                    case '^':
                        advance();
                        tokens.push_back(
                            Token{.type = TokenType::Power, .lexeme = "^", .location = location});
                        break;

                    case '=':
                        advance();
                        tokens.push_back(
                            Token{.type = TokenType::Assign, .lexeme = "=", .location = location});
                        break;

                    case '(':
                        advance();
                        tokens.push_back(Token{
                            .type = TokenType::LeftParen,
                            .lexeme = "(",
                            .location = location,
                        });
                        break;

                    case ')':
                        advance();
                        tokens.push_back(Token{
                            .type = TokenType::RightParen,
                            .lexeme = ")",
                            .location = location,
                        });
                        break;

                    case ':':
                        advance();
                        tokens.push_back(
                            Token{.type = TokenType::Colon, .lexeme = ":", .location = location});
                        break;

                    case ';':
                        advance();
                        tokens.push_back(Token{
                            .type = TokenType::Semicolon,
                            .lexeme = ";",
                            .location = location,
                        });
                        break;

                    default:
                        throw std::runtime_error(
                            "scanner error at line " + std::to_string(location.line) + ", column " +
                            std::to_string(location.column) + ": unknown character: " + ch);
                }

                previousTokenIsGoto = false;
                break;
            }
        }
    }

    if (tokens.empty() || tokens.back().type != TokenType::End)
    {
        tokens.push_back(Token{
            .type = TokenType::End,
            .lexeme = "#",
            .location = SourceLocation{.line = line, .column = column},
        });
    }

    return tokens;
}
