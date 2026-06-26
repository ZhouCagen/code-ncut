#include "SymbolTable.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

int SymbolTable::insertIdentifier(const std::string &value)
{
    return insertOrGet(identifiers_, identifierIndexes_, value);
}

int SymbolTable::insertConstant(const std::string &value)
{
    return insertOrGet(constants_, constantIndexes_, value);
}

int SymbolTable::insertLabel(const std::string &value)
{
    return insertOrGet(labels_, labelIndexes_, value);
}

int SymbolTable::insertOrGet(std::vector<std::string> &values,
                             std::unordered_map<std::string, int> &indexes,
                             const std::string &value)
{
    auto iterator = indexes.find(value);

    if (iterator != indexes.end())
        return iterator->second;

    int index = static_cast<int>(values.size());

    values.push_back(value);
    indexes.emplace(value, index);

    return index;
}

void SymbolTable::writeToFile(const std::string &path) const
{
    fs::path outputPath(path);

    if (!outputPath.parent_path().empty())
        fs::create_directories(outputPath.parent_path());

    std::ofstream output(path);

    if (!output.is_open())
        throw std::runtime_error("cannot create symbol table file: " + path);

    output << "Identifier Table:\n";
    for (std::size_t i = 0; i < identifiers_.size(); i++)
        output << i << ": " << identifiers_[i] << '\n';

    output << "\nConstant Table:\n";
    for (std::size_t i = 0; i < constants_.size(); i++)
        output << i << ": " << constants_[i] << '\n';

    output << "\nLabel Table:\n";
    for (std::size_t i = 0; i < labels_.size(); i++)
        output << i << ": " << labels_[i] << '\n';
}
