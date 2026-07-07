#include "utils.h"
#include <cstdio>
#include <cwchar>
#include <windows.h>
#include <Shlobj.h>

std::wstring Utils::utf8ToWide(std::string_view str)
{
    if (str.empty())
    {
        return {};
    }

    int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()),
                                   nullptr, 0);
    if (size <= 0)
    {
        return {};
    }

    std::wstring result(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()),
                        result.data(), size);
    return result;
}

std::string Utils::wideToUtf8(std::wstring_view str)
{
    if (str.empty())
    {
        return {};
    }

    int size = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()),
                                   nullptr, 0, nullptr, nullptr);
    if (size <= 0)
    {
        return {};
    }

    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()),
                        result.data(), size, nullptr, nullptr);
    return result;
}

void Utils::createConsole(const char* title)
{
    static bool consoleCreated = false;
    if (!consoleCreated)
    {
        consoleCreated = AllocConsole() != 0 || GetLastError() == ERROR_ACCESS_DENIED;
    }
    if (!consoleCreated)
    {
        return;
    }

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);

    SetConsoleTitleA(title);
}

std::string Utils::getModulePath(void* module)
{
    std::wstring result;
    result.resize(MAX_PATH);
    while (true)
    {
        DWORD len = GetModuleFileNameW(static_cast<HMODULE>(module), result.data(),
                                       static_cast<DWORD>(result.size()));
        if (len == 0)
        {
            return {};
        }
        if (len < result.size())
        {
            result.resize(len);
            break;
        }
        result.resize(result.size() * 2);
    }
    return wideToUtf8(result);
}

std::string Utils::getClipboardText()
{
    if (!OpenClipboard(nullptr))
    {
        return {};
    }

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData == nullptr)
    {
        CloseClipboard();
        return {};
    }

    const wchar_t* rawText = static_cast<const wchar_t*>(GlobalLock(hData));
    if (rawText == nullptr)
    {
        CloseClipboard();
        return {};
    }

    std::string text = wideToUtf8(rawText);
    GlobalUnlock(hData);
    CloseClipboard();
    return text;
}

std::optional<std::filesystem::path> Utils::getLocalAppDataPath()
{
    PWSTR rawPath = nullptr;

    if (FAILED(SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, nullptr,
                                    &rawPath)))
    {
        return std::nullopt;
    }

    std::filesystem::path path{rawPath};
    CoTaskMemFree(rawPath);

    return path;
}