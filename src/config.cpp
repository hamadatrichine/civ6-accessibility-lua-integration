#include "config.h"
#include "utils.h"

std::string Config::getValue(const std::string& section, const std::string& key,
                             const std::string& defaultValue)
{
    const char* value =
        mIni.GetValue(section.c_str(), key.c_str(), defaultValue.c_str());
    return value;
}

bool Config::setValue(const std::string& section, const std::string& key,
                      const std::string& value)
{
    SI_Error err = mIni.SetValue(section.c_str(), key.c_str(), value.c_str());
    if (err < 0)
    {
        spdlog::error("Failed to set config value: {}.{} = {}", section, key, value);
        return false;
    }

    err = mIni.SaveFile(getConfigFilepath().c_str());
    if (err < 0)
    {
        spdlog::error("Failed to save config file: {}", getConfigFilepath());
        return false;
    }

    return true;
}

std::string Config::getConfigFilepath()
{
    auto localAppDataPath = Utils::getLocalAppDataPath();
    if (!localAppDataPath)
    {
        return {};
    }

    auto configFilePath = *localAppDataPath / "Firaxis Games" /
                          "Sid Meier's Civilization VI" /
                          "civ6-accessibility-integration.ini";

    return configFilePath.string();
}