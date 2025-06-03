#pragma once
#include <filesystem>
#include <optional>
#include <string>

namespace awawa {
std::optional<std::string> loadTextFile(std::filesystem::path path);
}