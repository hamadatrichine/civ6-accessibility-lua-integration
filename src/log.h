#pragma once

#include <memory>
#include <string>

namespace spdlog
{
class logger;
} // namespace spdlog

namespace Log
{
void initialize();
void flush();
std::string filePath();
bool isDevelopmentLoggingEnabled();
std::shared_ptr<spdlog::logger> get();
} // namespace Log
