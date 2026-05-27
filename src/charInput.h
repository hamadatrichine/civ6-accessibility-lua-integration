#pragma once

#include "luaBindings.h"

namespace CharInput
{
void callHandler(const char* utf8Text);
int registerHandler(LuaBindings::lua_State* L);
int unregisterHandler(LuaBindings::lua_State* L);
} // namespace CharInput
