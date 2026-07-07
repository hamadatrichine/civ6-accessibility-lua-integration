#pragma once

#include "luaBindings.h"

namespace LuaMethods
{
int tolkOutput(LuaBindings::lua_State* L);
int tolkSpeak(LuaBindings::lua_State* L);
int tolkSilence(LuaBindings::lua_State* L);
int tolkBraille(LuaBindings::lua_State* L);
int tolkIsSpeaking(LuaBindings::lua_State* L);
int tolkIsLoaded(LuaBindings::lua_State* L);
int tolkHasSpeech(LuaBindings::lua_State* L);
int tolkHasBraille(LuaBindings::lua_State* L);
int tolkPreferSapi(LuaBindings::lua_State* L);
int tolkDetectScreenReader(LuaBindings::lua_State* L);
int registerCharInputHandler(LuaBindings::lua_State* L);
int unregisterCharInputHandler(LuaBindings::lua_State* L);
int getClipboardText(LuaBindings::lua_State* L);
int getConfigValue(LuaBindings::lua_State* L);
int setConfigValue(LuaBindings::lua_State* L);
} // namespace LuaMethods
