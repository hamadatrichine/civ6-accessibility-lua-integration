#pragma once

#include <string>
#include <string_view>

namespace Utils
{
std::wstring utf8ToWide(std::string_view str);
std::string wideToUtf8(std::wstring_view str);
void createConsole(const char* title);
std::string getModulePath(void* module);
std::string getClipboardText();
} // namespace Utils
