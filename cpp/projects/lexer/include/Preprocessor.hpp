#pragma once

#include <string>

class Preprocessor
{
  public:
    std::string preprocessFile(const std::string &inputPath, const std::string &outputPath) const;

  private:
    std::string readAndJoinContinuedLines(const std::string &inputPath) const;
    std::string removeComments(const std::string &source) const;
    std::string normalizeSpaces(const std::string &source) const;
    void writeFile(const std::string &outputPath, const std::string &content) const;
};
