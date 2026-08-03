#pragma once

#include <string>
#include <thread>
#include <mutex>

class UpdateManager
{
  public:
    static UpdateManager& instance()
    {
        static UpdateManager manager;
        return manager;
    }

    const std::string getLatestVersion();
    void startCheck();

  private:
    UpdateManager() = default;

    void checkThread();

    std::string getTagName(const std::string& json) const;

  private:
    mutable std::mutex mMutex;
    std::string mLatestVersion;
};
