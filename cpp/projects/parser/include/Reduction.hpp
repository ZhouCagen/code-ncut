#pragma once

#include <string>
#include <vector>

enum class Production
{
    Program = 0,

    StatementToAssignment = 1,
    StatementToIf = 2,
    StatementToGoto = 3,
    StatementToLabeledAssignment = 4,

    LabeledAssignment = 5,
    Assignment = 6,

    IfThen = 7,
    IfThenElse = 8,

    Goto = 9,

    BoolExpression = 10,

    Add = 11,
    Subtract = 12,
    Multiply = 13,
    Divide = 14,
    Power = 15,

    FactorIdentifier = 16,
    FactorConstant = 17,
    FactorParenthesized = 18
};

struct Reduction
{
    Production production;
    std::string description;
};

class ReductionRecorder
{
  public:
    void record(Production production);
    void writeToFile(const std::string &path) const;

    const std::vector<Reduction> &reductions() const;

  private:
    std::vector<Reduction> reductions_;
};

int productionNumber(Production production);
std::string productionDescription(Production production);
