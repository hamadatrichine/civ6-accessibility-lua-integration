#include "httpClient.h"
#include <Windows.h>
#include <vector>
#include <winhttp.h>

bool Http::get(const std::wstring& host, const std::wstring& path,
               std::string& response)
{
    response.clear();

    HINTERNET hSession =
        WinHttpOpen(L"Civ6 Accessibility", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession)
    {
        return false;
    }

    HINTERNET hConnect =
        WinHttpConnect(hSession, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);

    if (!hConnect)
    {
        WinHttpCloseHandle(hSession);
        return false;
    }

    HINTERNET hRequest =
        WinHttpOpenRequest(hConnect, L"GET", path.c_str(), nullptr, WINHTTP_NO_REFERER,
                           WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    if (!hRequest)
    {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                            WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
    {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    if (!WinHttpReceiveResponse(hRequest, nullptr))
    {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    DWORD size = 0;

    do
    {
        size = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &size))
            break;

        if (size == 0)
            break;

        std::vector<char> buffer(size);

        DWORD downloaded = 0;

        if (!WinHttpReadData(hRequest, buffer.data(), size, &downloaded))
        {
            break;
        }

        response.append(buffer.data(), downloaded);

    } while (size > 0);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return !response.empty();
}