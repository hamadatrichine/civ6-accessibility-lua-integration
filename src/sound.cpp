#include "sound.h"
#include <spdlog/spdlog.h>

namespace
{
ma_attenuation_model toMaAttenuationModel(AttenuationModel model)
{
    switch (model)
    {
    case AttenuationModel::None:
        return ma_attenuation_model_none;

    case AttenuationModel::Inverse:
        return ma_attenuation_model_inverse;

    case AttenuationModel::Linear:
        return ma_attenuation_model_linear;

    case AttenuationModel::Exponential:
        return ma_attenuation_model_exponential;
    }

    return ma_attenuation_model_inverse;
}
} // namespace

Sound::Sound(const std::string& filePath)
{
    mFilePath = filePath;
}

Sound::~Sound()
{
    shutdown();
}

bool Sound::initialize(ma_engine* engine)
{
    mInitialized = ma_sound_init_from_file(engine, mFilePath.c_str(), 0, nullptr,
                                           nullptr, &mSound) == MA_SUCCESS;

    restoreState();
    return mInitialized;
}

void Sound::shutdown()
{
    if (!mInitialized)
    {
        return;
    }

    mWasPlaying = isPlaying();
    spdlog::info("sound uninit");
    ma_sound_uninit(&mSound);
    mInitialized = false;
}

void Sound::restoreState()
{
    if (!mInitialized)
    {
        return;
    }

    setVolume(mVolume);
    setPan(mPan);
    setPitch(mPitch);
    SetLooping(mLooping);
    setPosition(mPx, mPy, mPz);
    setDirection(mDx, mDy, mDz);
    setVelocity(mVx, mVy, mVz);
    setMinDistance(mMinDist);
    setMaxDistance(mMaxDist);
    setAttenuationModel(mAttenuationModel);
    setSpatializationEnabled(mSpatializationEnabled);

    if (mWasPlaying)
    {
        play();
    }
}

void Sound::play()
{
    ma_sound_start(&mSound);
}

void Sound::pause()
{
    ma_sound_stop(&mSound);
}

void Sound::stop()
{
    ma_sound_stop(&mSound);
    ma_sound_seek_to_pcm_frame(&mSound, 0);
}

void Sound::SetLooping(bool looping)
{
    mLooping = looping;
    ma_sound_set_looping(&mSound, looping ? MA_TRUE : MA_FALSE);
}

bool Sound::IsLooping() const
{
    return ma_sound_is_looping(&mSound) == MA_TRUE;
}

void Sound::setVolume(float volume)
{
    mVolume = volume;
    ma_sound_set_volume(&mSound, volume);
}

float Sound::getVolume() const
{
    return ma_sound_get_volume(&mSound);
}

void Sound::setPitch(float pitch)
{
    mPitch = pitch;
    ma_sound_set_pitch(&mSound, pitch);
}

float Sound::getPitch() const
{
    return ma_sound_get_pitch(&mSound);
}

void Sound::setPosition(float x, float y, float z)
{
    mPx = x;
    mPy = y;
    mPz = z;
    ma_sound_set_position(&mSound, x, y, z);
}

void Sound::getPosition(float& x, float& y, float& z) const
{
    auto pos = ma_sound_get_position(&mSound);
    x = pos.x;
    y = pos.y;
    z = pos.z;
}

void Sound::setDirection(float x, float y, float z)
{
    mDx = x;
    mDy = y;
    mDz = z;
    ma_sound_set_direction(&mSound, x, y, z);
}

void Sound::setVelocity(float x, float y, float z)
{
    mVx = x;
    mVy = y;
    mVz = z;
    ma_sound_set_velocity(&mSound, x, y, z);
}

void Sound::setSpatializationEnabled(bool enabled)
{
    mSpatializationEnabled = enabled;
    ma_sound_set_spatialization_enabled(&mSound, enabled ? MA_TRUE : MA_FALSE);
}

bool Sound::isSpatializationEnabled() const
{
    return ma_sound_is_spatialization_enabled(&mSound) == MA_TRUE;
}

void Sound::setMinDistance(float distance)
{
    mMinDist = distance;
    ma_sound_set_min_distance(&mSound, distance);
}

void Sound::setMaxDistance(float distance)
{
    mMaxDist = distance;
    ma_sound_set_max_distance(&mSound, distance);
}

void Sound::setAttenuationModel(AttenuationModel model)
{
    mAttenuationModel = model;
    ma_sound_set_attenuation_model(&mSound, toMaAttenuationModel(model));
}

void Sound::setPan(float pan)
{
    mPan = pan;
    ma_sound_set_pan(&mSound, pan);
}

float Sound::getPan() const
{
    return ma_sound_get_pan(&mSound);
}

bool Sound::isPlaying() const
{
    return ma_sound_is_playing(&mSound) != 0;
}
