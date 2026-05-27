#include "log.h"
#include "utils.h"
#include <Windows.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <functional> // Added: Necessary for std::function
#include <string>
#include <vector>

namespace
{
constexpr const wchar_t* LOG_FILE_NAME = L"civ6-accessibility-integration.log";
constexpr const wchar_t* CIV6_LOGS_SUBDIR =
    L"Firaxis Games\\Sid Meier's Civilization VI\\Logs";

std::wstring configuredLogDirectory()
{
    DWORD required = GetEnvironmentVariableW(L"LOCALAPPDATA", nullptr, 0);

    if (required == 0)
    {
        return {};
    }

    std::wstring path(required, L'\0');
    DWORD written = GetEnvironmentVariableW(L"LOCALAPPDATA", path.data(), required);
    if (written == 0 || written >= required)
    {
        return {};
    }

    path.resize(written);
    if (!path.empty() && path.back() != L'\\')
    {
        path += L'\\';
    }
    path += CIV6_LOGS_SUBDIR;
    return path;
}

std::wstring configuredLogPath()
{
    std::wstring path = configuredLogDirectory();
    if (path.empty())
    {
        return LOG_FILE_NAME;
    }

    CreateDirectoryW(path.c_str(), nullptr);

    if (!path.empty() && path.back() != L'\\')
    {
        path += L'\\';
    }
    path += LOG_FILE_NAME;
    return path;
}

void writeDebugString(const std::string& message)
{
    OutputDebugStringA("civ6-accessibility-integration logging error: ");
    OutputDebugStringA(message.c_str());
    OutputDebugStringA("\n");
}

template <typename Factory>
void addSink(std::vector<spdlog::sink_ptr>& sinks, Factory&& factory)
{
    try
    {
        sinks.push_back(factory());
    }
    catch (const std::exception& error)
    {
        writeDebugString(error.what());
    }
    catch (...)
    {
        writeDebugString("unknown sink initialization failure");
    }
}

void installFallbackLogger()
{
    try
    {
        auto logger = std::make_shared<spdlog::logger>(
            "civ6_accessibility", std::make_shared<spdlog::sinks::null_sink_mt>());
        spdlog::set_default_logger(logger);
    }
    catch (...)
    {
    }
}
} // namespace

void Log::initialize()
{
    try
    {
        if (spdlog::get("civ6_accessibility"))
        {
            return;
        }

        spdlog::set_error_handler(writeDebugString);

        std::vector<spdlog::sink_ptr> sinks;
#if CAI_DEV_LOGGING
        addSink(sinks,
                [] { return std::make_shared<spdlog::sinks::stdout_color_sink_mt>(); });
        addSink(sinks, [] { return std::make_shared<spdlog::sinks::msvc_sink_mt>(); });
#endif
        addSink(sinks, [] {
            return std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                configuredLogPath(), 1024 * 1024, 3);
        });

        if (sinks.empty())
        {
            sinks.push_back(std::make_shared<spdlog::sinks::null_sink_mt>());
        }

        auto logger = std::make_shared<spdlog::logger>("civ6_accessibility",
                                                       sinks.begin(), sinks.end());
        logger->set_error_handler(writeDebugString);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

#if CAI_DEV_LOGGING
        logger->set_level(spdlog::level::debug);
#else
        logger->set_level(spdlog::level::info);
#endif
        logger->flush_on(spdlog::level::info);

        spdlog::set_default_logger(logger);
    }
    catch (const std::exception& error)
    {
        writeDebugString(error.what());
        installFallbackLogger();
    }
    catch (...)
    {
        writeDebugString("unknown logger initialization failure");
        installFallbackLogger();
    }
}

void Log::flush()
{
    if (auto logger = spdlog::default_logger())
    {
        logger->flush();
    }
}

std::string Log::filePath()
{
    return Utils::wideToUtf8(configuredLogPath());
}

bool Log::isDevelopmentLoggingEnabled()
{
#if CAI_DEV_LOGGING
    return true;
#else
    return false;
#endif
}

std::shared_ptr<spdlog::logger> Log::get()
{
    initialize();
    return spdlog::default_logger();
}
