#include "Lexer.hpp"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_set>

namespace fs = std::filesystem;

namespace
{
bool isIdentifierStart(char ch)
{
    return std::isalpha(static_cast<unsigned char>(ch));
}

bool isIdentifierBody(char ch)
{
    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

bool isDigit(char ch)
{
    return std::isdigit(static_cast<unsigned char>(ch));
}

bool isKeyword(const std::string &word)
{
    static const std::unordered_set<std::string> keywords = {"IF", "THEN", "ELSE", "GOTO"};

    return keywords.contains(word);
}

bool isDoubleOperator(const std::string &value)
{
    static const std::unordered_set<std::string> operators = {">=", "<=", "<>"};

    return operators.contains(value);
}

bool isSingleOperator(char ch)
{
    static const std::set<char> operators = {'+', '-', '*', '/', '=', '>', '<', '^'};

    return operators.contains(ch);
}

bool isPunctuation(char ch)
{
    static const std::set<char> punctuations = {'(', ')', ':', ';'};

    return punctuations.contains(ch);
}
} // namespace

Lexer::Lexer(SymbolTable &symbolTable) : symbolTable_(symbolTable)
{
}

std::vector<Token> Lexer::scan(std::string_view source)
{
    std::vector<Token> tokens;

    std::size_t i = 0;
    std::size_t line = 1;
    std::size_t column = 1;

    bool previousTokenIsGoto = false;

    auto currentLocation = [&]() { return SourceLocation{.line = line, .column = column}; };

    auto advance = [&]()
    {
        char ch = source[i];
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

    while (i < source.size())
    {
        char ch = source[i];

        if (std::isspace(static_cast<unsigned char>(ch)))
        {
            advance();
            continue;
        }

        SourceLocation startLocation = currentLocation();

        if (ch == '#')
        {
            advance();

            tokens.push_back(Token{.type = TokenType::End,
                                   .lexeme = "EOF",
                                   .location = startLocation,
                                   .tableIndex = std::nullopt});

            break;
        }

        State state;

        if (isIdentifierStart(ch))
            state = State::Identifier;
        else if (isDigit(ch))
            state = State::Number;
        else
            state = State::OperatorOrPunctuation;

        switch (state)
        {
            case State::Identifier:
            {
                std::string word;

                while (i < source.size() && isIdentifierStart(source[i]))
                    word += advance();

                if (i < source.size() && isKeyword(word))
                {
                    tokens.push_back(Token{.type = TokenType::Keyword,
                                           .lexeme = word,
                                           .location = startLocation,
                                           .tableIndex = std::nullopt});
                }
                else
                {
                    int tableIndex = symbolTable_.insertIdentifier(word);
                    tokens.push_back(Token{.type = TokenType::Identifier,
                                           .lexeme = word,
                                           .location = startLocation,
                                           .tableIndex = tableIndex});
                }
                break;
            }

            case State::Number:
            {
                std::string number;

                while (i < source.size() && isDigit(source[i]))
                    number += advance();

                if (i < source.size() && isIdentifierStart(source[i]))
                {
                    throw makeError(startLocation,
                                    "number cannot be followed directly by identifier character: " +
                                        number);
                }

                bool isLabel = false;

                if (previousTokenIsGoto)
                    isLabel = true;

                if (i < source.size() && source[i] == ':')
                    isLabel = true;

                if (isLabel)
                {
                    int tableIndex = symbolTable_.insertLabel(number);

                    tokens.push_back(Token{.type = TokenType::Label,
                                           .lexeme = number,
                                           .location = startLocation,
                                           .tableIndex = static_cast<std::size_t>(tableIndex)});
                }
                else
                {
                    int tableIndex = symbolTable_.insertConstant(number);

                    tokens.push_back(Token{.type = TokenType::Constant,
                                           .lexeme = number,
                                           .location = startLocation,
                                           .tableIndex = static_cast<std::size_t>(tableIndex)});
                }

                previousTokenIsGoto = false;
                break;
            }

            case State::OperatorOrPunctuation:
            {
                if (i + 1 < source.size())
                {
                    std::string twoChars;
                    twoChars += source[i];
                    twoChars += source[i + 1];

                    if (isDoubleOperator(twoChars))
                    {
                        advance();
                        advance();

                        tokens.push_back(Token{.type = TokenType::Operator,
                                               .lexeme = twoChars,
                                               .location = startLocation,
                                               .tableIndex = std::nullopt});

                        previousTokenIsGoto = false;
                        break;
                    }
                }

                if (isSingleOperator(ch))
                {
                    std::string value;
                    value += advance();

                    tokens.push_back(Token{.type = TokenType::Operator,
                                           .lexeme = value,
                                           .location = startLocation,
                                           .tableIndex = std::nullopt});

                    previousTokenIsGoto = false;
                    break;
                }

                if (isPunctuation(ch))
                {
                    std::string value;
                    value += advance();

                    tokens.push_back(Token{.type = TokenType::Punctuation,
                                           .lexeme = value,
                                           .location = startLocation,
                                           .tableIndex = std::nullopt});

                    previousTokenIsGoto = false;
                    break;
                }

                throw makeError(startLocation, std::string("unknown character: ") + ch);
            }
        }
    }

    return tokens;
}

void Lexer::writeTokens(const std::vector<Token> &tokens, const std::string &path) const
{
    fs::path outputPath(path);

    if (!outputPath.parent_path().empty())
        fs::create_directories(outputPath.parent_path());

    std::ofstream output(path);

    if (!output.is_open())
        throw std::runtime_error("cannot create token file: " + path);

    for (const auto &token : tokens)
    {
        if (token.type == TokenType::End)
        {
            output << "(#,EOF)\n";
        }
        else
        {
            output << '(' << tokenTypeCode(token.type) << ',' << token.lexeme << ")\n";
        }
    }
}

std::runtime_error Lexer::makeError(const SourceLocation &location,
                                    const std::string &message) const
{
    return std::runtime_error("lexical error at line " + std::to_string(location.line) +
                              ", column " + std::to_string(location.column) + ": " + message);
}
