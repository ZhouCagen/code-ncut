#pragma once
#include "museflow/models.hpp"
namespace museflow {
const json& blueprintSchema();
MusicBlueprint parseModelBlueprint(const json& value);
void stampPrompt(MusicBlueprint& blueprint, const std::string& source);
bool promptIsCurrent(const MusicBlueprint& blueprint);
}
