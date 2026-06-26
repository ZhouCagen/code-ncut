#include "Parser.hpp"
#include "Scanner.hpp"
#include "Token.hpp"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::string readFile(const std::string &path)
{
    std::ifstream input(path);

    if (!input.is_open())
        throw std::runtime_error("cannot open input file: " + path);

    std::ostringstream buffer;
    buffer << input.rdbuf();

    return buffer.str();
}

void writeFailureResult(const std::string &path, const std::string &message)
{
    fs::path outputPath(path);

    if (!outputPath.parent_path().empty())
        fs::create_directories(outputPath.parent_path());

    std::ofstream output(path);

    if (!output.is_open())
        throw std::runtime_error("cannot create parser result file: " + path);

    output << "Syntax analysis failed.\n";
    output << "Error: " << message << '\n';
}

void writeScannedTokens(const std::vector<Token> &tokens, const std::string &path)
{
    fs::path outputPath(path);

    if (!outputPath.parent_path().empty())
        fs::create_directories(outputPath.parent_path());

    std::ofstream output(path);

    if (!output.is_open())
        throw std::runtime_error("cannot create token debug file: " + path);

    output << "Scanned tokens:\n";

    for (const auto &token : tokens)
    {
        output << token.location.line << ':' << token.location.column << "  "
               << tokenTypeName(token.type) << "  " << token.lexeme << '\n';
    }
}

int main(int argc, char *argv[])
{
    const std::string outputDirectory = "output";
    const std::string reductionPath = outputDirectory + "/reductions.txt";
    const std::string resultPath = outputDirectory + "/parser_result.txt";
    const std::string tokenDebugPath = outputDirectory + "/scanned_tokens.txt";

    try
    {
        std::string inputPath = "preprocessed.txt";

        if (argc >= 2)
            inputPath = argv[1];

        fs::create_directories(outputDirectory);

        std::string source = readFile(inputPath);

        Scanner scanner(source);
        std::vector<Token> tokens = scanner.scan();

        writeScannedTokens(tokens, tokenDebugPath);

        Parser parser(tokens);
        parser.parseProgram();

        parser.writeReductions(reductionPath);
        parser.writeResult(resultPath);

        std::cout << "语法分析完成\n";
        std::cout << "输入文件: " << inputPath << '\n';
        std::cout << "Token调试文件: " << tokenDebugPath << '\n';
        std::cout << "归约序列文件: " << reductionPath << '\n';
        std::cout << "分析结果文件: " << resultPath << '\n';
    }
    catch (const std::exception &error)
    {
        try
        {
            writeFailureResult(resultPath, error.what());
        }
        catch (...)
        {
        }

        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
