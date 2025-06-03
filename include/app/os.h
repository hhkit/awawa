#pragma once
#include <filesystem>

namespace awawa {
std::filesystem::path find_executable();
std::filesystem::path find_executable_directory();
} // namespace awawa