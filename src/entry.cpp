#include <Windows.h>
#include <Tolk.h>
#include <spdlog/spdlog.h>
#include "luaBindings.h"
#include "hooks.h"
#include "log.h"
#include "utils.h"
#include <exception>
#include <string>
#include "audioManager.h"
#include "updateManager.h"

static HMODULE currentModule = nullptr;

static unsigned int initialize()
{
#if CAI_DEV_LOGGING
    Utils::createConsole("Civ6 Injected DLL Console");
#endif
    Log::initialize();
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();
    std::string modulePath = Utils::getModulePath(currentModule);
    spdlog::info("DLL injected into process ID: {}, thread ID: {}", processId,
                 threadId);
    spdlog::info("Module path: {}", modulePath);
    spdlog::info("Log file path: {}", Log::filePath());
    spdlog::info("Logging mode: {}",
                 Log::isDevelopmentLoggingEnabled() ? "development" : "production");
    Log::flush();

    Tolk_TrySAPI(true);
    spdlog::debug("Tolk SAPI fallback enabled");
    Tolk_Load();
    spdlog::info("Tolk status: loaded={}, speech={}, braille={}", Tolk_IsLoaded(),
                 Tolk_HasSpeech(), Tolk_HasBraille());
    if (!AudioManager::instance().initialize())
    {
        spdlog::error("Failed to initialize audio manager");
    }

    if (LuaBindings::importSymbols())
    {
        spdlog::info("Successfully imported Lua symbols");
        if (Hooks::install())
        {
            spdlog::info("Hooks installed successfully");
            spdlog::info("Listening for Lua state");
        }
        else
        {
            spdlog::error("Failed to install hooks");
        }
    }
    else
    {
        spdlog::error("Failed to import Lua symbols");
    }
    UpdateManager::instance().startCheck();
    Log::flush();
    // Note(Hamada): return 3 for no device, so that the game gives up  on trying to use
    // the dll
    return 3;
}

extern "C" __declspec(dllexport) unsigned int __stdcall LFX_Initialize()
{
    try
    {
        return initialize();
    }
    catch (const std::exception& error)
    {
        std::string message = "civ6-accessibility-integration startup exception: ";
        message += error.what();
        message += "\n";
        OutputDebugStringA(message.c_str());
    }
    catch (...)
    {
        OutputDebugStringA(
            "civ6-accessibility-integration startup exception: unknown\n");
    }
    return 3;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        currentModule = module;
        DisableThreadLibraryCalls(module);
    }
    return TRUE;
}
