#include "updateManager.h"
#include "httpClient.h"
#include <spdlog/spdlog.h>

const std::string UpdateManager::getLatestVersion()
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mLatestVersion;
}

void UpdateManager::startCheck()
{
    std::thread(&UpdateManager::checkThread, this).detach();
}

void UpdateManager::checkThread()
{
    std::string json;
    spdlog::info("Checking for updates...");
    if (Http::get(
            L"api.github.com",
            L"/repos/flat-arther/Civ-VI-Accessibility-Integration/releases/latest",
            json))
    {
        auto version = getTagName(json);

        if (!version.empty() && version.front() == 'v')
        {
            version.erase(0, 1);
        }
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mLatestVersion = version;
        }
    }
    else
    {
        spdlog::error("Failed to check for updates");
    }
}

std::string UpdateManager::getTagName(const std::string& json) const
{
    auto key = json.find("\"tag_name\"");
    if (key == std::string::npos)
        return {};

    auto colon = json.find(':', key);
    if (colon == std::string::npos)
        return {};

    auto firstQuote = json.find('"', colon);
    if (firstQuote == std::string::npos)
        return {};

    auto secondQuote = json.find('"', firstQuote + 1);
    if (secondQuote == std::string::npos)
        return {};

    return json.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}
