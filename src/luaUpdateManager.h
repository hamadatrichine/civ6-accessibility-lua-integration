#pragma once

#include "luaBindings.h"

namespace LuaUpdateManager
{
void registerApiFunctions(LuaBindings::lua_State* L);
} // namespace LuaUpdateManager