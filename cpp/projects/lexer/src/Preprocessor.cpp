#include "Preprocessor.hpp"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

std::string Preprocessor::preprocessFile(const std::string &inputPath,
                                         const std::string &outputPath) const
{
    std::string joined = readAndJoinContinuedLines(inputPath);
    std::string noComment = removeComments(joined);
    std::string normalized = normalizeSpaces(noComment);

    if (normalized.empty() || normalized.back() != '#')
        normalized += '#';

    writeFile(outputPath, normalized);

    return normalized;
}

std::string Preprocessor::readAndJoinContinuedLines(const std::string &inputPath) const
{
    std::ifstream input(inputPath);
    if (!input.is_open())
        throw std::runtime_error("cannot open source file: " + inputPath);

    std::string result;
    std::string line;
    int continuationCount = 0;

    while (std::getline(input, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (!line.empty() && line.back() == '\\')
        {
            line.pop_back();
            result += line;
            continuationCount++;

            if (continuationCount > 3)
                throw std::runtime_error("continuation lines exceed 3");
        }
        else
        {
            result += line;
            result += '\n';
            continuationCount = 0;
        }
    }
    return result;
}

std::string Preprocessor::removeComments(const std::string &source) const
{
    std::string result;
    bool inBlockComment = false;

    for (std::size_t i = 0; i < source.size(); i++)
    {
        if (!inBlockComment && i + 1 < source.size() && source[i] == '/' && source[i + 1] == '*')
        {
            inBlockComment = true;
            result += ' ';
            i++;
            continue;
        }

        if (inBlockComment && i + 1 < source.size() && source[i] == '*' && source[i + 1] == '/')
        {
            inBlockComment = false;
            i++;
            continue;
        }

        if (!inBlockComment)
            result += source[i];
    }

    if (inBlockComment)
        throw std::runtime_error("unclosed block comment");

    return result;
}

std::string Preprocessor::normalizeSpaces(const std::string &source) const
{
    std::string result;
    bool previousIsSpace = false;

    for (char ch : source)
    {
        if (std::isspace(static_cast<unsigned char>(ch)))
        {
            if (!previousIsSpace && !result.empty())
            {
                result += ' ';
                previousIsSpace = true;
            }
        }
        else
        {
            result += ch;
            previousIsSpace = false;
        }
    }

    while (!result.empty() && result.back() == ' ')
        result.pop_back();

    return result;
}

void Preprocessor::writeFile(const std::string &outputPath, const std::string &content) const
{
    fs::path path(outputPath);

    if (!path.parent_path().empty())
        fs::create_directories(path.parent_path());

    std::ofstream output(outputPath);

    if (!output.is_open())
        throw std::runtime_error("cannot create output file: " + outputPath);

    output << content;
}
