#include "luaAudio.h"
#include "audioManager.h"

using namespace LuaBindings;
namespace
{
static int lua_loadSound(lua_State* L)
{
    const char* filePath = checklstring(L, 1, nullptr);

    auto handleOpt = AudioManager::instance().loadSound(filePath);
    if (!handleOpt)
    {
        pushnil(L);
        return 1;
    }

    AudioManager::Handle handle = *handleOpt;
    pushinteger(L, handle);
    return 1;
}

static int lua_destroySound(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    bool success = AudioManager::instance().destroySound(handle);
    pushboolean(L, success);
    return 1;
}

static int lua_playSound(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    AudioManager::instance().playSound(handle);
    return 0;
}

static int lua_pauseSound(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    AudioManager::instance().pauseSound(handle);
    return 0;
}

static int lua_stopSound(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    AudioManager::instance().stopSound(handle);
    return 0;
}

static int lua_setSoundVolume(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float volume = static_cast<float>(checknumber(L, 2));
    AudioManager::instance().setSoundVolume(handle, volume);
    return 0;
}

static int lua_getSoundVolume(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float volume = AudioManager::instance().getSoundVolume(handle);
    pushnumber(L, volume);
    return 1;
}

static int lua_setMasterVolume(lua_State* L)
{
    float volume = checknumber(L, 1);
    AudioManager::instance().setMasterVolume(volume);
    return 0;
}

static int lua_getMasterVolume(lua_State* L)
{
    float volume = AudioManager::instance().getMasterVolume();
    pushnumber(L, volume);
    return 1;
}

static int lua_setSoundLooping(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    bool looping = toboolean(L, 2);
    AudioManager::instance().setSoundLooping(handle, looping);
    return 0;
}

static int lua_isSoundLooping(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    bool looping = AudioManager::instance().isSoundLooping(handle);
    pushboolean(L, looping);
    return 1;
}

static int lua_setSoundPitch(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float pitch = static_cast<float>(checknumber(L, 2));
    AudioManager::instance().setSoundPitch(handle, pitch);
    return 0;
}

static int lua_getSoundPitch(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float pitch = AudioManager::instance().getSoundPitch(handle);
    pushnumber(L, pitch);
    return 1;
}

static int lua_setSoundPosition(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float x = static_cast<float>(checknumber(L, 2));
    float y = static_cast<float>(checknumber(L, 3));
    float z = static_cast<float>(checknumber(L, 4));
    AudioManager::instance().setSoundPosition(handle, x, y, z);
    return 0;
}

static int lua_getSoundPosition(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float x, y, z;
    AudioManager::instance().getSoundPosition(handle, x, y, z);
    pushnumber(L, x);
    pushnumber(L, y);
    pushnumber(L, z);
    return 3;
}

static int lua_setSoundDirection(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float x = static_cast<float>(checknumber(L, 2));
    float y = static_cast<float>(checknumber(L, 3));
    float z = static_cast<float>(checknumber(L, 4));
    AudioManager::instance().setSoundDirection(handle, x, y, z);
    return 0;
}

static int lua_setSoundVelocity(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float x = static_cast<float>(checknumber(L, 2));
    float y = static_cast<float>(checknumber(L, 3));
    float z = static_cast<float>(checknumber(L, 4));
    AudioManager::instance().setSoundVelocity(handle, x, y, z);
    return 0;
}

static int lua_setSoundSpatializationEnabled(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    bool enabled = toboolean(L, 2);
    AudioManager::instance().setSoundSpatializationEnabled(handle, enabled);
    return 0;
}

static int lua_isSoundSpatializationEnabled(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    bool enabled = AudioManager::instance().isSoundSpatializationEnabled(handle);
    pushboolean(L, enabled);
    return 1;
}

static int lua_setSoundMinDistance(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float distance = static_cast<float>(checknumber(L, 2));
    AudioManager::instance().setSoundMinDistance(handle, distance);
    return 0;
}

static int lua_setSoundMaxDistance(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float distance = static_cast<float>(checknumber(L, 2));
    AudioManager::instance().setSoundMaxDistance(handle, distance);
    return 0;
}

static int lua_setSoundAttenuationModel(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    int model = checkinteger(L, 2);
    AudioManager::instance().setSoundAttenuationModel(
        handle, static_cast<AttenuationModel>(model));
    return 0;
}

static int lua_setSoundPan(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float pan = static_cast<float>(checknumber(L, 2));
    AudioManager::instance().setSoundPan(handle, pan);
    return 0;
}

static int lua_getSoundPan(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    float pan = AudioManager::instance().getSoundPan(handle);
    pushnumber(L, pan);
    return 1;
}

static int lua_isSoundPlaying(lua_State* L)
{
    AudioManager::Handle handle = static_cast<AudioManager::Handle>(checkinteger(L, 1));
    bool isPlaying = AudioManager::instance().isSoundPlaying(handle);
    pushboolean(L, isPlaying);
    return 1;
}

static int lua_setListenerPosition(lua_State* L)
{
    float x = static_cast<float>(checknumber(L, 1));
    float y = static_cast<float>(checknumber(L, 2));
    float z = static_cast<float>(checknumber(L, 3));
    AudioManager::instance().setListenerPosition(x, y, z);
    return 0;
}

static int lua_setListenerDirection(lua_State* L)
{
    float x = static_cast<float>(checknumber(L, 1));
    float y = static_cast<float>(checknumber(L, 2));
    float z = static_cast<float>(checknumber(L, 3));
    AudioManager::instance().setListenerDirection(x, y, z);
    return 0;
}

static int lua_setListenerUp(lua_State* L)
{
    float x = static_cast<float>(checknumber(L, 1));
    float y = static_cast<float>(checknumber(L, 2));
    float z = static_cast<float>(checknumber(L, 3));
    AudioManager::instance().setListenerUp(x, y, z);
    return 0;
}

static int lua_setListenerVelocity(lua_State* L)
{
    float x = static_cast<float>(checknumber(L, 1));
    float y = static_cast<float>(checknumber(L, 2));
    float z = static_cast<float>(checknumber(L, 3));
    AudioManager::instance().setListenerVelocity(x, y, z);
    return 0;
}

static int lua_audioUpdate(lua_State* L)
{
    AudioManager::instance().audioUpdate();
    return 0;
}
} // anonymous namespace

void LuaAudio::registerApiFunctions(LuaBindings::lua_State* L)
{
    pushClosure(L, lua_loadSound, -2, "LoadSound");
    pushClosure(L, lua_destroySound, -2, "DestroySound");

    pushClosure(L, lua_playSound, -2, "PlaySound");
    pushClosure(L, lua_pauseSound, -2, "PauseSound");
    pushClosure(L, lua_stopSound, -2, "StopSound");

    pushClosure(L, lua_setSoundVolume, -2, "SetSoundVolume");
    pushClosure(L, lua_getSoundVolume, -2, "GetSoundVolume");

    pushClosure(L, lua_setSoundLooping, -2, "SetSoundLooping");
    pushClosure(L, lua_isSoundLooping, -2, "IsSoundLooping");

    pushClosure(L, lua_setSoundPitch, -2, "SetSoundPitch");
    pushClosure(L, lua_getSoundPitch, -2, "GetSoundPitch");

    pushClosure(L, lua_setSoundPan, -2, "SetSoundPan");
    pushClosure(L, lua_getSoundPan, -2, "GetSoundPan");

    pushClosure(L, lua_setSoundPosition, -2, "SetSoundPosition");
    pushClosure(L, lua_getSoundPosition, -2, "GetSoundPosition");

    pushClosure(L, lua_setSoundDirection, -2, "SetSoundDirection");
    pushClosure(L, lua_setSoundVelocity, -2, "SetSoundVelocity");

    pushClosure(L, lua_setSoundSpatializationEnabled, -2,
                "SetSoundSpatializationEnabled");
    pushClosure(L, lua_isSoundSpatializationEnabled, -2,
                "IsSoundSpatializationEnabled");

    pushClosure(L, lua_setSoundMinDistance, -2, "SetSoundMinDistance");
    pushClosure(L, lua_setSoundMaxDistance, -2, "SetSoundMaxDistance");
    pushClosure(L, lua_setSoundAttenuationModel, -2, "SetSoundAttenuationModel");

    pushClosure(L, lua_isSoundPlaying, -2, "IsSoundPlaying");

    pushClosure(L, lua_setListenerPosition, -2, "SetListenerPosition");
    pushClosure(L, lua_setListenerDirection, -2, "SetListenerDirection");
    pushClosure(L, lua_setListenerUp, -2, "SetListenerUp");
    pushClosure(L, lua_setListenerVelocity, -2, "SetListenerVelocity");

    pushClosure(L, lua_setMasterVolume, -2, "SetMasterVolume");
    pushClosure(L, lua_getMasterVolume, -2, "GetMasterVolume");
    pushClosure(L, lua_audioUpdate, -2, "AudioUpdate");
}
