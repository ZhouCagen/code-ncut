#pragma once

#include "Reduction.hpp"
#include "Token.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Parser
{
  public:
    explicit Parser(std::vector<Token> tokens);

    void parseProgram();

    void writeReductions(const std::string &path) const;
    void writeResult(const std::string &path) const;

  private:
    void parseStatement();

    void parseAssignment();
    void parseLabeledAssignment();
    void parseIfStatement();
    void parseGotoStatement();

    void parseBoolExpression();
    void parseArithmeticExpression(int minPrecedence = 0);
    void parsePrimary();

    std::vector<Token> tokens_;
    std::size_t current_ = 0;

    ReductionRecorder reductions_;
    bool parseSuccess_ = false;
};
