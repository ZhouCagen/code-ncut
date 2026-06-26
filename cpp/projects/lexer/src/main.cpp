#include "Lexer.hpp"
#include "Preprocessor.hpp"
#include "SymbolTable.hpp"
#include "Token.hpp"

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    try
    {
        std::string sourcePath = "source.txt";

        if (argc >= 2)
            sourcePath = argv[1]; // ./lexer test.txt

        const std::string outputDirectory = "output";
        const std::string preprocessedPath = outputDirectory + "/preprocessed.txt";
        const std::string tokenPath = outputDirectory + "/tokens.txt";
        const std::string tablePath = outputDirectory + "/tables.txt";

        fs::create_directories(outputDirectory);

        Preprocessor preprocessor;
        std::string preprocessed = preprocessor.preprocessFile(sourcePath, preprocessedPath);

        SymbolTable symbolTable;
        Lexer lexer(symbolTable);

        std::vector<Token> tokens = lexer.scan(preprocessed);

        lexer.writeTokens(tokens, tokenPath);
        symbolTable.writeToFile(tablePath);

        std::cout << "词法分析完成\n";
        std::cout << "源程序文件: " << sourcePath << '\n';
        std::cout << "预处理文件: " << preprocessedPath << '\n';
        std::cout << "二元式文件: " << tokenPath << '\n';
        std::cout << "符号表文件: " << tablePath << '\n';
    }
    catch (const std::exception &error)
    {
        std::cerr << "error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
