#include "audioManager.h"
#include <miniaudio_libvorbis.h>
#include <miniaudio_libopus.h>
#include <spdlog/spdlog.h>

static void audioNotificationCallback(const ma_device_notification* notification)
{
    auto& audio = AudioManager::instance();

    if (audio.mRecreatingEngine)
        return;
    switch (notification->type)
    {
    case ma_device_notification_type_started:
        spdlog::info("Audio device started");
        break;

    case ma_device_notification_type_stopped:
        spdlog::info("Audio device stopped");
        AudioManager::instance().requestEngineRecreation();
        break;

    case ma_device_notification_type_rerouted:
        spdlog::info("Audio device rerouted");
        AudioManager::instance().requestEngineRecreation();
        break;

    case ma_device_notification_type_interruption_began:
        spdlog::info("Audio interruption began");
        break;

    case ma_device_notification_type_interruption_ended:
        spdlog::info("Audio interruption ended");
        AudioManager::instance().requestEngineRecreation();
        break;

    case ma_device_notification_type_unlocked:
        spdlog::info("Audio device unlocked");
        break;
    default:
        spdlog::info("Audio device unknown notification");
        break;
    }
}

bool AudioManager::initialize()
{
    if (mInitialized)
    {
        return false;
    }

    ma_resource_manager_config rmConfig = ma_resource_manager_config_init();

    ma_decoding_backend_vtable* backends[] = {ma_decoding_backend_libvorbis,
                                              ma_decoding_backend_libopus};

    rmConfig.ppCustomDecodingBackendVTables = backends;
    rmConfig.customDecodingBackendCount = std::size(backends);

    if (ma_resource_manager_init(&rmConfig, &mResourceManager) != MA_SUCCESS)
    {
        spdlog::error("Failed to initialize resource manager.");
        return false;
    }

    if (!initializeEngine())
    {
        ma_resource_manager_uninit(&mResourceManager);
        return false;
    }

    mInitialized = true;
    spdlog::debug("AudioManager initialized.");
    return true;
}

void AudioManager::shutdown()
{
    if (!mInitialized)
    {
        return;
    }

    mSounds.clear();
    shutdownEngine();
    ma_resource_manager_uninit(&mResourceManager);
    mInitialized = false;
    spdlog::debug("AudioManager shutdown.");
}

bool AudioManager::recreateEngine()
{
    mRecreatingEngine = true;
    if (!mInitialized)
    {
        spdlog::error("Attempted to recreate an uninitialized audio engine");
        return false;
    }
    for (auto& [handle, sound] : mSounds)
    {
        spdlog::info("shutting down sound {}", handle);
        sound->shutdown();
    }
    shutdownEngine();

    if (!initializeEngine())
    {
        spdlog::error("Failed to recreate audio engine");
        return false;
    }
    for (auto& [handle, sound] : mSounds)
    {
        if (!sound->initialize(&mEngine))
        {
            spdlog::warn("Failed to recreate sound {}", handle);
        }
    }

    spdlog::info("Audio engine recreated.");
    mRecreatingEngine = false;
    return true;
}

std::optional<AudioManager::Handle> AudioManager::loadSound(const std::string& filePath)
{
    if (mRecreatingEngine)
        return std::nullopt;

    auto sound = std::make_unique<Sound>(filePath);
    spdlog::debug("Loading sound from file: {}", filePath);
    if (!sound->initialize(&mEngine))
    {
        return std::nullopt;
    }

    Handle handle = mNextHandle++;

    mSounds.emplace(handle, std::move(sound));

    spdlog::debug("Sound loaded successfully, assigned handle: {}", handle);

    return handle;
}

bool AudioManager::destroySound(Handle handle)
{
    spdlog::debug("Destroying sound {}", handle);
    return mSounds.erase(handle) > 0;
}

void AudioManager::playSound(Handle handle)
{
    if (mRecreatingEngine)
        return;
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->play();
    }
}

void AudioManager::pauseSound(Handle handle)
{
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->pause();
    }
}

void AudioManager::stopSound(Handle handle)
{
    if (mRecreatingEngine)
        return;
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->stop();
    }
}

void AudioManager::setSoundVolume(Handle handle, float volume)
{
    if (mRecreatingEngine)
        return;
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setVolume(volume);
    }
}

float AudioManager::getSoundVolume(Handle handle) const
{
    if (mRecreatingEngine)
        return 0.0f;
    Sound* sound = findSound(handle);
    if (sound)
    {
        return sound->getVolume();
    }
    return 0.0f;
}

void AudioManager::setMasterVolume(float volume)
{
    if (mRecreatingEngine)
        return;
    mVolume = volume;
    ma_engine_set_volume(&mEngine, volume);
}

float AudioManager::getMasterVolume()
{
    if (mRecreatingEngine)
        return 0.0f;
    return ma_engine_get_volume(&mEngine);
}

void AudioManager::setSoundLooping(Handle handle, bool looping)
{
    if (mRecreatingEngine)
        return;
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->SetLooping(looping);
    }
}

bool AudioManager::isSoundLooping(Handle handle) const
{
    if (mRecreatingEngine)
        return false;
    Sound* sound = findSound(handle);
    if (sound)
    {
        return sound->IsLooping();
    }
    return false;
}

void AudioManager::setSoundPitch(Handle handle, float pitch)
{
    if (mRecreatingEngine)
        return;
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setPitch(pitch);
    }
}

float AudioManager::getSoundPitch(Handle handle) const
{
    if (mRecreatingEngine)
        return 0.0f;
    Sound* sound = findSound(handle);
    if (sound)
    {
        return sound->getPitch();
    }
    return 0.0f;
}

void AudioManager::setSoundPosition(Handle handle, float x, float y, float z)
{
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setPosition(x, y, z);
    }
}

void AudioManager::getSoundPosition(Handle handle, float& x, float& y, float& z) const
{
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->getPosition(x, y, z);
    }
}

void AudioManager::setSoundDirection(Handle handle, float x, float y, float z)
{
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setDirection(x, y, z);
    }
}

void AudioManager::setSoundVelocity(Handle handle, float x, float y, float z)
{
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setVelocity(x, y, z);
    }
}

void AudioManager::setSoundSpatializationEnabled(Handle handle, bool enabled)
{
    if (mRecreatingEngine)
        return;
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setSpatializationEnabled(enabled);
    }
}

bool AudioManager::isSoundSpatializationEnabled(Handle handle) const
{
    if (mRecreatingEngine)
        return false;
    Sound* sound = findSound(handle);
    if (sound)
    {
        return sound->isSpatializationEnabled();
    }
    return false;
}

void AudioManager::setSoundMinDistance(Handle handle, float distance)
{
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setMinDistance(distance);
    }
}

void AudioManager::setSoundMaxDistance(Handle handle, float distance)
{
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setMaxDistance(distance);
    }
}

void AudioManager::setSoundAttenuationModel(Handle handle, AttenuationModel model)
{
    if (mRecreatingEngine)
        return;
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setAttenuationModel(model);
    }
}

void AudioManager::setSoundPan(Handle handle, float pan)
{
    if (mRecreatingEngine)
        return;
    Sound* sound = findSound(handle);
    if (sound)
    {
        sound->setPan(pan);
    }
}

float AudioManager::getSoundPan(Handle handle) const
{
    if (mRecreatingEngine)
        return 0.0f;
    Sound* sound = findSound(handle);
    if (sound)
    {
        return sound->getPan();
    }
    return 0.0f;
}

bool AudioManager::isSoundPlaying(Handle handle) const
{
    Sound* sound = findSound(handle);
    if (sound)
    {
        return sound->isPlaying();
    }
    return false;
}

void AudioManager::setListenerPosition(float x, float y, float z)
{
    mPx = x;
    mPy = y;
    mPz = z;
    ma_engine_listener_set_position(&mEngine, 0, x, y, z);
}

void AudioManager::setListenerDirection(float x, float y, float z)
{
    mDx = x;
    mDy = y;
    mDz = z;
    ma_engine_listener_set_direction(&mEngine, 0, x, y, z);
}

void AudioManager::setListenerUp(float x, float y, float z)
{
    mUx = x;
    mUy = y;
    mUz = z;
    ma_engine_listener_set_world_up(&mEngine, 0, x, y, z);
}

void AudioManager::setListenerVelocity(float x, float y, float z)
{
    mVx = x;
    mVy = y;
    mVz = z;
    ma_engine_listener_set_velocity(&mEngine, 0, x, y, z);
}

void AudioManager::audioUpdate()
{
    if (mRecreatingEngine)
        return;
    auto device = ma_engine_get_device(&mEngine);
    auto state = ma_device_get_state(device);
    spdlog::info("device state = {}", (int)state);
    if (state == ma_device_state_stopped)
    {
        spdlog::info("Trying to restart device...");
        auto result = ma_device_start(device);
        spdlog::info("Result = {}", (int)result);
        if (result == MA_UNAVAILABLE)
        {
            spdlog::info("making new engine after device unavailable");
            recreateEngine();
        }
    }
}

bool AudioManager::initializeEngine()
{
    ma_engine_config config = ma_engine_config_init();
    config.pResourceManager = &mResourceManager;
    config.notificationCallback = audioNotificationCallback;

    if (ma_engine_init(&config, &mEngine) != MA_SUCCESS)
    {
        spdlog::error("Audio engine initialization failed");
        return false;
    }
    spdlog::debug("Audio engine initialized");
    restoreState();

    return true;
}

void AudioManager::shutdownEngine()
{
    if (mRecreatingEngine)
        return;
    spdlog::info("shutting down engine");
    ma_engine_uninit(&mEngine);
    spdlog::info("Audio engine shutdown");
}

void AudioManager::requestEngineRecreation()
{
    mEngineRecreateRequested = true;
}

void AudioManager::restoreState()
{
    if (mRecreatingEngine)
        return;
    setMasterVolume(mVolume);
    setListenerPosition(mPx, mPy, mPz);
    setListenerDirection(mDx, mDy, mDz);
    setListenerUp(mUx, mUy, mUz);
    setListenerVelocity(mVx, mVy, mVz);
}

Sound* AudioManager::findSound(Handle handle) const
{
    auto it = mSounds.find(handle);
    if (it != mSounds.end())
    {
        return it->second.get();
    }
    return nullptr;
}
