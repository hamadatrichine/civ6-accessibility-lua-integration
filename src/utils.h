#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <filesystem>

namespace Utils
{
std::wstring utf8ToWide(std::string_view str);
std::string wideToUtf8(std::wstring_view str);
void createConsole(const char* title);
std::string getModulePath(void* module);
bool isGameWindowFocused();
std::string getClipboardText();
std::optional<std::filesystem::path> getLocalAppDataPath();
} // namespace Utils
