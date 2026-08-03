#pragma once

#include <string>

namespace Http
{
bool get(const std::wstring& host, const std::wstring& path, std::string& response);
}