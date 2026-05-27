#include "charInput.h"
#include "hooks.h"
#include "luaApi.h"
#include "luaBindings.h"
#include "utils.h"
#include <MinHook.h>
#include <cstddef>
#include <spdlog/spdlog.h>
#include <windows.h>

static LuaBindings::loadbuffer_t originalLoadBuffer = nullptr;
static bool hooksInstalled = false;
static bool loadbufferHookEnabled = false;
static bool dispatchMessageHookEnabled = false;

static bool failInstall(const char* message)
{
    spdlog::error("{}", message);
    if (loadbufferHookEnabled)
    {
        spdlog::debug("Disabling loadbuffer hook after install failure");
        MH_DisableHook(reinterpret_cast<LPVOID>(LuaBindings::loadbuffer));
        loadbufferHookEnabled = false;
    }
    if (dispatchMessageHookEnabled)
    {
        spdlog::debug("Disabling DispatchMessageW hook after install failure");
        MH_DisableHook(reinterpret_cast<void*>(DispatchMessageW));
        dispatchMessageHookEnabled = false;
    }
    MH_Uninitialize();
    hooksInstalled = false;
    return false;
}

static int __cdecl loadbufferHook(LuaBindings::lua_State* l, const char* buff,
                                  size_t size, const char* name)
{
    LuaBindings::getfield(l, LuaBindings::GLOBALS_INDEX, "ExposedMembers");
    if (LuaBindings::isTable(l, -1))
    {
        LuaApi::injectAccessibilityTable(l);
        spdlog::info("Injected ExposedMembers.CAI into Lua: {}", (void*)l);
        if (MH_DisableHook(reinterpret_cast<LPVOID>(LuaBindings::loadbuffer)) == MH_OK)
        {
            loadbufferHookEnabled = false;
        }
    }
    else
    {
        spdlog::debug("Skipped CAI injection; ExposedMembers is not a table yet");
    }
    LuaBindings::pop(l, 1);
    return originalLoadBuffer(l, buff, size, name);
}

typedef LRESULT(WINAPI* DispatchMessageW_t)(const MSG* lpMsg);
static DispatchMessageW_t originalDispatchMessageW = nullptr;
static WNDPROC originalWndProc = nullptr;
static LRESULT CALLBACK WndProcHook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CHAR: {
        wchar_t ch = static_cast<wchar_t>(wParam);
        static wchar_t highSurrogate = 0;
        std::wstring fullChar;
        // NOTE(Hamada): pass backspace and other control characters if needed
        if (ch < 0x20 || ch == 0x7F)
        {
            break;
        }
        else if (IS_HIGH_SURROGATE(ch))
        {
            highSurrogate = ch;
            break;
        }
        else if (IS_LOW_SURROGATE(ch))
        {
            if (highSurrogate != 0)
            {
                fullChar += highSurrogate;
                fullChar += ch;
                highSurrogate = 0;
            }
            else
            {
                break;
            }
        }
        else
        {
            fullChar += ch;
            highSurrogate = 0;
        }

        std::string utf8 = Utils::wideToUtf8(fullChar);
        if (utf8.empty())
        {
            break;
        }
        CharInput::callHandler(utf8.c_str());
    }
    break;
    default:
        break;
    }
    return CallWindowProc(originalWndProc, hwnd, msg, wParam, lParam);
}

static HWND gameWindow = nullptr;
LRESULT WINAPI HookedDispatchMessageW(const MSG* msg)
{

    if (msg && msg->hwnd)
    {
        if (!gameWindow)
        {
            HWND root = GetAncestor(msg->hwnd, GA_ROOT);

            DWORD pid = 0;
            GetWindowThreadProcessId(root, &pid);

            if (pid == GetCurrentProcessId() && IsWindowVisible(root))
            {
                int titleLength = GetWindowTextLengthW(root);

                if (titleLength > 0)
                {
                    std::wstring title(titleLength + 1, L'\0');
                    int copied = GetWindowTextW(root, title.data(), title.size());
                    title.resize(copied);

                    gameWindow = root;

                    spdlog::info("Detected game window: {} | {}", (void*)gameWindow,
                                 Utils::wideToUtf8(title));
                }
            }
        }

        if (gameWindow && !originalWndProc)
        {
            spdlog::debug("Installing WndProc hook for detected game window");
            originalWndProc = (WNDPROC)SetWindowLongPtr(gameWindow, GWLP_WNDPROC,
                                                        (LONG_PTR)WndProcHook);

            if (originalWndProc)
            {
                spdlog::info("WndProc hooked successfully");
                if (MH_DisableHook(reinterpret_cast<void*>(DispatchMessageW)) == MH_OK)
                {
                    dispatchMessageHookEnabled = false;
                }
            }
            else
            {
                spdlog::error("Failed to hook WndProc: {}", GetLastError());
            }
        }
    }

    return originalDispatchMessageW(msg);
}

bool Hooks::install()
{
    if (hooksInstalled)
    {
        spdlog::debug("Hooks already installed");
        return true;
    }

    spdlog::debug("Initializing MinHook");
    if (MH_Initialize() != MH_OK)
    {
        spdlog::error("Failed to initialize MinHook");
        return false;
    }
    spdlog::debug("Creating loadbuffer hook");
    if (MH_CreateHook(reinterpret_cast<void*>(LuaBindings::loadbuffer),
                      reinterpret_cast<void*>(&loadbufferHook),
                      reinterpret_cast<LPVOID*>(&originalLoadBuffer)) != MH_OK)
    {
        return failInstall("Failed to create hook for loadbuffer.");
    }
    if (MH_EnableHook(reinterpret_cast<void*>(LuaBindings::loadbuffer)) != MH_OK)
    {
        return failInstall("Failed to enable hook for loadbuffer.");
    }
    loadbufferHookEnabled = true;
    spdlog::debug("Creating DispatchMessageW hook");
    if (MH_CreateHook(DispatchMessageW, &HookedDispatchMessageW,
                      reinterpret_cast<LPVOID*>(&originalDispatchMessageW)) != MH_OK)
    {
        return failInstall("Failed to create hook for DispatchMessageW.");
    }
    if (MH_EnableHook(DispatchMessageW) != MH_OK)
    {
        return failInstall("Failed to enable hook for DispatchMessageW.");
    }
    dispatchMessageHookEnabled = true;
    hooksInstalled = true;
    spdlog::debug("Hook installation complete");
    return true;
}

void Hooks::uninstall()
{
    if (!hooksInstalled)
    {
        spdlog::debug("Hooks uninstall skipped; hooks are not installed");
        return;
    }

    if (loadbufferHookEnabled)
    {
        spdlog::debug("Disabling loadbuffer hook");
        MH_DisableHook(reinterpret_cast<LPVOID>(LuaBindings::loadbuffer));
        loadbufferHookEnabled = false;
    }
    if (dispatchMessageHookEnabled)
    {
        spdlog::debug("Disabling DispatchMessageW hook");
        MH_DisableHook(reinterpret_cast<void*>(DispatchMessageW));
        dispatchMessageHookEnabled = false;
    }
    if (gameWindow && originalWndProc)
    {
        spdlog::debug("Restoring original WndProc");
        SetWindowLongPtr(gameWindow, GWLP_WNDPROC,
                         reinterpret_cast<LONG_PTR>(originalWndProc));
        originalWndProc = nullptr;
        gameWindow = nullptr;
    }
    MH_Uninitialize();
    hooksInstalled = false;
    originalLoadBuffer = nullptr;
    originalDispatchMessageW = nullptr;
}
