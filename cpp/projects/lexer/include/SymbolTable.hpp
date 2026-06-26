#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class SymbolTable
{
  public:
    int insertIdentifier(const std::string &value);
    int insertConstant(const std::string &value);
    int insertLabel(const std::string &value);

    void writeToFile(const std::string &path) const;

  private:
    int insertOrGet(std::vector<std::string> &values, std::unordered_map<std::string, int> &indexes,
                    const std::string &value);

    std::vector<std::string> identifiers_;
    std::vector<std::string> constants_;
    std::vector<std::string> labels_;

    std::unordered_map<std::string, int> identifierIndexes_;
    std::unordered_map<std::string, int> constantIndexes_;
    std::unordered_map<std::string, int> labelIndexes_;
};
