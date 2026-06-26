#include "Reduction.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

int productionNumber(Production production)
{
    return static_cast<int>(production);
}

std::string productionDescription(Production production)
{
    switch (production)
    {
        case Production::Program:
            return "Program -> StatementList #";

        case Production::StatementToAssignment:
            return "Statement -> Assignment";
        case Production::StatementToIf:
            return "Statement -> IfStatement";
        case Production::StatementToGoto:
            return "Statement -> GotoStatement";
        case Production::StatementToLabeledAssignment:
            return "Statement -> LabeledAssignment";

        case Production::LabeledAssignment:
            return "LabeledAssignment -> Label : Assignment";
        case Production::Assignment:
            return "Assignment -> Identifier = ArithmeticExpression";

        case Production::IfThen:
            return "IfStatement -> IF BoolExpression THEN Statement";
        case Production::IfThenElse:
            return "IfStatement -> IF BoolExpression THEN Statement ELSE Statement";

        case Production::Goto:
            return "GotoStatement -> GOTO Label";

        case Production::BoolExpression:
            return "BoolExpression -> ArithmeticExpression Relop ArithmeticExpression";

        case Production::Add:
            return "ArithmeticExpression -> ArithmeticExpression + Term";
        case Production::Subtract:
            return "ArithmeticExpression -> ArithmeticExpression - Term";
        case Production::Multiply:
            return "Term -> Term * Factor";
        case Production::Divide:
            return "Term -> Term / Factor";
        case Production::Power:
            return "Term -> Factor ^ Term";

        case Production::FactorIdentifier:
            return "Factor -> Identifier";
        case Production::FactorConstant:
            return "Factor -> Constant";
        case Production::FactorParenthesized:
            return "Factor -> ( ArithmeticExpression )";
    }

    return "Unknown production";
}

void ReductionRecorder::record(Production production)
{
    reductions_.push_back(
        Reduction{.production = production, .description = productionDescription(production)});
}

void ReductionRecorder::writeToFile(const std::string &path) const
{
    fs::path outputPath(path);

    if (!outputPath.parent_path().empty())
        fs::create_directories(outputPath.parent_path());

    std::ofstream output(path);

    if (!output.is_open())
        throw std::runtime_error("cannot create reduction file: " + path);

    output << "Reduction sequence:\n";

    for (const auto &reduction : reductions_)
    {
        output << productionNumber(reduction.production) << ". " << reduction.description << '\n';
    }
}

const std::vector<Reduction> &ReductionRecorder::reductions() const
{
    return reductions_;
}
