#pragma once

#include <miniaudio.h>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <string>
#include <optional>
#include "sound.h"

class AudioManager
{
  public:
    using Handle = uint32_t;

    static AudioManager& instance()
    {
        static AudioManager manager;
        return manager;
    }

    bool initialize();
    void shutdown();
    void requestEngineRecreation();

    std::optional<Handle> loadSound(const std::string& filePath);
    bool destroySound(Handle handle);

    void playSound(Handle handle);
    void pauseSound(Handle handle);
    void stopSound(Handle handle);

    void setSoundVolume(Handle handle, float volume);
    float getSoundVolume(Handle handle) const;

    void setMasterVolume(float volume);
    float getMasterVolume();
    void setSoundLooping(Handle handle, bool looping);
    bool isSoundLooping(Handle handle) const;

    void setSoundPitch(Handle handle, float pitch);
    float getSoundPitch(Handle handle) const;

    void setSoundPosition(Handle handle, float x, float y, float z);
    void getSoundPosition(Handle handle, float& x, float& y, float& z) const;

    void setSoundDirection(Handle handle, float x, float y, float z);

    void setSoundVelocity(Handle handle, float x, float y, float z);

    void setSoundSpatializationEnabled(Handle handle, bool enabled);
    bool isSoundSpatializationEnabled(Handle handle) const;

    void setSoundMinDistance(Handle handle, float distance);
    void setSoundMaxDistance(Handle handle, float distance);

    void setSoundAttenuationModel(Handle handle, AttenuationModel model);

    void setSoundPan(Handle handle, float pan);
    float getSoundPan(Handle handle) const;

    bool isSoundPlaying(Handle handle) const;

    void setListenerPosition(float x, float y, float z);

    void setListenerDirection(float x, float y, float z);

    void setListenerUp(float x, float y, float z);

    void setListenerVelocity(float x, float y, float z);

    void audioUpdate();

    std::atomic_bool mRecreatingEngine = false;

  private:
    AudioManager() = default;
    bool initializeEngine();
    void shutdownEngine();
    bool recreateEngine();
    void restoreState();

    Sound* findSound(Handle handle) const;

  private:
    ma_engine mEngine{};
    ma_resource_manager mResourceManager{};
    bool mInitialized = false;
    Handle mNextHandle = 1;
    std::atomic_bool mEngineRecreateRequested = false;

    std::unordered_map<Handle, std::unique_ptr<Sound>> mSounds;

    // Note(Hamada): cached variables for engine recreation
    float mVolume = 1.0f;
    float mPx, mPy, mPz = 0.0f;
    float mDx, mDy = 0.0f;
    float mDz = -1.0f;
    float mUx, mUy = 0.0f;
    float mUz = 1.0f;
    float mVx, mVy, mVz = 0.0f;
};
