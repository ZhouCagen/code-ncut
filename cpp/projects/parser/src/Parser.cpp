#include "Parser.hpp"
#include "ParseError.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace fs = std::filesystem;

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens))
{
    if (tokens_.empty() || tokens_.back().type != TokenType::End)
    {
        tokens_.push_back(Token{
            .type = TokenType::End,
            .lexeme = "#",
            .location = SourceLocation{},
        });
    }
}

void Parser::parseProgram()
{
    current_ = 0;
    parseSuccess_ = false;
    reductions_ = ReductionRecorder{};

    while (tokens_[current_].type == TokenType::Semicolon)
        current_++;

    while (tokens_[current_].type != TokenType::End)
    {
        parseStatement();

        while (tokens_[current_].type == TokenType::Semicolon)
            current_++;
    }

    reductions_.record(Production::Program);
    parseSuccess_ = true;
}

void Parser::parseStatement()
{
    switch (tokens_[current_].type)
    {
        case TokenType::Identifier:
            parseAssignment();
            reductions_.record(Production::StatementToAssignment);
            return;

        case TokenType::Label:
            parseLabeledAssignment();
            reductions_.record(Production::StatementToLabeledAssignment);
            return;

        case TokenType::If:
            parseIfStatement();
            reductions_.record(Production::StatementToIf);
            return;

        case TokenType::Goto:
            parseGotoStatement();
            reductions_.record(Production::StatementToGoto);
            return;

        default:
        {
            const Token &token = tokens_[current_];

            throw ParseError(token.location, "expected statement, got `" + token.lexeme + "`");
        }
    }
}

void Parser::parseAssignment()
{
    if (tokens_[current_].type != TokenType::Identifier)
    {
        const Token &token = tokens_[current_];

        throw ParseError(token.location,
                         "expected identifier at beginning of assignment statement, got `" +
                             token.lexeme + "`");
    }

    current_++;

    if (tokens_[current_].type != TokenType::Assign)
    {
        const Token &token = tokens_[current_];

        throw ParseError(token.location, "expected = after identifier, got `" + token.lexeme + "`");
    }

    current_++;

    parseArithmeticExpression();

    reductions_.record(Production::Assignment);
}

void Parser::parseLabeledAssignment()
{
    if (tokens_[current_].type != TokenType::Label)
    {
        const Token &token = tokens_[current_];

        throw ParseError(token.location, "expected label, got `" + token.lexeme + "`");
    }

    current_++;

    if (tokens_[current_].type != TokenType::Colon)
    {
        const Token &token = tokens_[current_];

        throw ParseError(token.location, "expected : after label, got `" + token.lexeme + "`");
    }

    current_++;

    parseAssignment();

    reductions_.record(Production::LabeledAssignment);
}

void Parser::parseIfStatement()
{
    if (tokens_[current_].type != TokenType::If)
    {
        const Token &token = tokens_[current_];

        throw ParseError(token.location, "expected IF, got `" + token.lexeme + "`");
    }

    current_++;

    bool hasOuterParentheses = false;

    if (tokens_[current_].type == TokenType::LeftParen)
    {
        hasOuterParentheses = true;
        current_++;
    }

    parseBoolExpression();

    if (hasOuterParentheses)
    {
        if (tokens_[current_].type != TokenType::RightParen)
        {
            const Token &token = tokens_[current_];

            throw ParseError(token.location,
                             "expected ) after boolean expression, got `" + token.lexeme + "`");
        }

        current_++;
    }

    if (tokens_[current_].type != TokenType::Then)
    {
        const Token &token = tokens_[current_];

        throw ParseError(token.location,
                         "expected THEN after boolean expression, got `" + token.lexeme + "`");
    }

    current_++;

    parseStatement();

    while (tokens_[current_].type == TokenType::Semicolon)
        current_++;

    if (tokens_[current_].type == TokenType::Else)
    {
        current_++;

        parseStatement();

        reductions_.record(Production::IfThenElse);
    }
    else
    {
        reductions_.record(Production::IfThen);
    }
}

void Parser::parseGotoStatement()
{
    if (tokens_[current_].type != TokenType::Goto)
    {
        const Token &token = tokens_[current_];

        throw ParseError(token.location, "expected GOTO, got `" + token.lexeme + "`");
    }

    current_++;

    if (tokens_[current_].type != TokenType::Label)
    {
        const Token &token = tokens_[current_];

        throw ParseError(token.location, "expected label after GOTO, got `" + token.lexeme + "`");
    }

    current_++;

    reductions_.record(Production::Goto);
}

void Parser::parseBoolExpression()
{
    parseArithmeticExpression();

    if (!isRelationalOperator(tokens_[current_].type))
    {
        const Token &token = tokens_[current_];

        throw ParseError(token.location,
                         "expected relational operator in boolean expression, got `" +
                             token.lexeme + "`");
    }

    current_++;

    parseArithmeticExpression();

    reductions_.record(Production::BoolExpression);
}

void Parser::parseArithmeticExpression(int minPrecedence)
{
    parsePrimary();

    while (tokens_[current_].type != TokenType::End)
    {
        TokenType operatorType = tokens_[current_].type;

        if (operatorType == TokenType::Identifier || operatorType == TokenType::Label ||
            operatorType == TokenType::If || operatorType == TokenType::Goto ||
            operatorType == TokenType::Then || operatorType == TokenType::Else ||
            operatorType == TokenType::RightParen || operatorType == TokenType::Colon ||
            operatorType == TokenType::Semicolon || isRelationalOperator(operatorType))
        {
            break;
        }

        int operatorPrecedence = -1;

        switch (operatorType)
        {
            case TokenType::Plus:
            case TokenType::Minus:
                operatorPrecedence = 1;
                break;

            case TokenType::Star:
            case TokenType::Slash:
                operatorPrecedence = 2;
                break;

            case TokenType::Power:
                operatorPrecedence = 3;
                break;

            default:
            {
                const Token &token = tokens_[current_];

                throw ParseError(token.location,
                                 "invalid arithmetic operator, got `" + token.lexeme + "`");
            }
        }

        if (operatorPrecedence < minPrecedence)
            break;

        Token operatorToken = tokens_[current_];
        current_++;

        int nextMinPrecedence = operatorPrecedence + 1;

        if (operatorType == TokenType::Power)
            nextMinPrecedence = operatorPrecedence;

        parseArithmeticExpression(nextMinPrecedence);

        switch (operatorType)
        {
            case TokenType::Plus:
                reductions_.record(Production::Add);
                break;

            case TokenType::Minus:
                reductions_.record(Production::Subtract);
                break;

            case TokenType::Star:
                reductions_.record(Production::Multiply);
                break;

            case TokenType::Slash:
                reductions_.record(Production::Divide);
                break;

            case TokenType::Power:
                reductions_.record(Production::Power);
                break;

            default:
                throw ParseError(operatorToken.location,
                                 "invalid arithmetic operator, got `" + operatorToken.lexeme + "`");
        }
    }
}

void Parser::parsePrimary()
{
    switch (tokens_[current_].type)
    {
        case TokenType::Identifier:
            current_++;
            reductions_.record(Production::FactorIdentifier);
            return;

        case TokenType::Constant:
            current_++;
            reductions_.record(Production::FactorConstant);
            return;

        case TokenType::LeftParen:
        {
            current_++;

            parseArithmeticExpression();

            if (tokens_[current_].type != TokenType::RightParen)
            {
                const Token &token = tokens_[current_];

                throw ParseError(token.location, "expected ) after arithmetic expression, got `" +
                                                     token.lexeme + "`");
            }

            current_++;

            reductions_.record(Production::FactorParenthesized);
            return;
        }

        default:
        {
            const Token &token = tokens_[current_];

            throw ParseError(token.location,
                             "expected identifier, constant, or parenthesized expression, got `" +
                                 token.lexeme + "`");
        }
    }
}

void Parser::writeReductions(const std::string &path) const
{
    reductions_.writeToFile(path);
}

void Parser::writeResult(const std::string &path) const
{
    fs::path outputPath(path);

    if (!outputPath.parent_path().empty())
        fs::create_directories(outputPath.parent_path());

    std::ofstream output(path);

    if (!output.is_open())
        throw std::runtime_error("cannot create parser result file: " + path);

    if (parseSuccess_)
    {
        output << "Syntax analysis succeeded.\n";
        output << "Reduction count: " << reductions_.reductions().size() << '\n';
    }
    else
    {
        output << "Syntax analysis failed.\n";
    }
}
