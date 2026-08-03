#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include "SimpleIni.h"

class Config
{
  public:
    static Config& instance()
    {
        static Config config;
        return config;
    }

    std::string getValue(const std::string& section, const std::string& key,
                         const std::string& defaultValue);
    bool setValue(const std::string& section, const std::string& key,
                  const std::string& value);

  private:
    Config()
    {
        mIni.SetUnicode();
        auto err = mIni.LoadFile(getConfigFilepath().c_str());
        if (err < 0)
        {
            spdlog::error("Failed to load config file: {}", getConfigFilepath());
        }
        else
        {
            spdlog::info("Loaded config file: {}", getConfigFilepath());
        }
    }

    std::string getConfigFilepath();

  private:
    CSimpleIniA mIni;

  public:
    // TODO: move this to a global state class
    void* gameWindow = nullptr;
};