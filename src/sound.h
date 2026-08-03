#pragma once

#include <string>
#include <memory>
#include <limits>
#include <miniaudio.h>

enum class AttenuationModel
{
    None,
    Inverse,
    Linear,
    Exponential
};

class Sound
{
  public:
    Sound(const std::string& filePath);
    ~Sound();

    bool initialize(ma_engine* engine);
    void shutdown();

    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;

    Sound(Sound&&) = delete;
    Sound& operator=(Sound&&) = delete;

    void play();
    void pause();
    void stop();

    void SetLooping(bool looping);
    bool IsLooping() const;

    void setVolume(float volume);
    float getVolume() const;

    void setPitch(float pitch);
    float getPitch() const;

    void setPosition(float x, float y, float z);
    void getPosition(float& x, float& y, float& z) const;

    void setDirection(float x, float y, float z);

    void setVelocity(float x, float y, float z);

    void setSpatializationEnabled(bool enabled);
    bool isSpatializationEnabled() const;

    void setMinDistance(float distance);
    void setMaxDistance(float distance);

    void setAttenuationModel(AttenuationModel model);

    void setPan(float pan);
    float getPan() const;

    bool isPlaying() const;

  private:
    void restoreState();

  private:
    std::string mFilePath;
    bool mInitialized = false;
    ma_sound mSound;

    // Note(Hamada): cache values to apply when engine is restored
    float mVolume = 1.0f;
    float mPitch = 1.0f;
    float mPan = 0.0f;
    float mPx, mPy, mPz = 0.0f;
    float mDx, mDy = 0.0f;
    float mDz = 1.0f;
    float mVx, mVy, mVz = 0.0f;
    float mMinDist = 1.0f;
    float mMaxDist = std::numeric_limits<float>::max();
    bool mLooping = false;
    bool mWasPlaying = false;
    bool mSpatializationEnabled = false;
    AttenuationModel mAttenuationModel = AttenuationModel::Inverse;
};
