#include "luaBindings.h"
#include "utils.h"
#include <Windows.h>
#include <cstdint>
#include <spdlog/spdlog.h>

namespace
{
template <typename Function>
bool importSymbol(HMODULE module, Function& target, const char* exportName,
                  const char* friendlyName)
{
    target = reinterpret_cast<Function>(GetProcAddress(module, exportName));
    if (!target)
    {
        spdlog::error("Failed to import Lua symbol {} ({})", friendlyName, exportName);
        return false;
    }
    return true;
}
} // namespace

namespace LuaBindings
{
pushnamedcclosure_t pushnamedcclosure = nullptr;
setfield_t setfield = nullptr;
getfield_t getfield = nullptr;
gettop_t gettop = nullptr;
pop_t pop = nullptr;
ref_t ref = nullptr;
unref_t unref = nullptr;
rawgeti_t rawgeti = nullptr;
checktype_t checktype = nullptr;
checklstring_t checklstring = nullptr;
toboolean_t toboolean = nullptr;
type_t type = nullptr;
pushfstring_t pushfstring = nullptr;
pushinteger_t pushinteger = nullptr;
createtable_t createtable = nullptr;
dostring_t dostring = nullptr;
pcall_t pcall = nullptr;
loadbuffer_t loadbuffer = nullptr;

bool importSymbols()
{
    HMODULE hModule = GetModuleHandleW(LUA_DLL);
    if (hModule)
    {
        spdlog::debug("Importing Lua symbols from {}", Utils::wideToUtf8(LUA_DLL));
        bool imported = true;
        imported &= importSymbol(
            hModule, pushnamedcclosure,
            "?hksi_hks_pushnamedcclosure@@YAXPEAUlua_State@@P6AH0@ZHPEBDH@Z",
            "hksi_hks_pushnamedcclosure");
        imported &= importSymbol(hModule, setfield,
                                 "?hksi_lua_setfield@@YAXPEAUlua_State@@HPEBD@Z",
                                 "hksi_lua_setfield");
        imported &= importSymbol(hModule, getfield,
                                 "?hksi_lua_getfield@@YAXPEAUlua_State@@HPEBD@Z",
                                 "hksi_lua_getfield");
        imported &=
            importSymbol(hModule, gettop, "?hksi_lua_gettop@@YAHPEAUlua_State@@@Z",
                         "hksi_lua_gettop");
        imported &= importSymbol(hModule, pop, "?Pop@LuaState@LuaPlus@@QEAAXH@Z",
                                 "LuaPlus::LuaState::Pop");
        imported &=
            importSymbol(hModule, ref, "?luaL_ref@@YAHPEAUlua_State@@H@Z", "luaL_ref");
        imported &= importSymbol(hModule, unref, "?luaL_unref@@YAXPEAUlua_State@@HH@Z",
                                 "luaL_unref");
        imported &=
            importSymbol(hModule, rawgeti, "?hksi_lua_rawgeti@@YAXPEAUlua_State@@HH@Z",
                         "hksi_lua_rawgeti");
        imported &=
            importSymbol(hModule, checktype, "?luaL_checktype@@YAXPEAUlua_State@@HH@Z",
                         "luaL_checktype");
        imported &=
            importSymbol(hModule, checklstring,
                         "?hksi_luaL_checklstring@@YAPEBDPEAUlua_State@@HPEA_K@Z",
                         "hksi_luaL_checklstring");
        imported &= importSymbol(hModule, toboolean,
                                 "?hksi_lua_toboolean@@YAHPEAUlua_State@@H@Z",
                                 "hksi_lua_toboolean");
        imported &= importSymbol(hModule, type, "?hksi_lua_type@@YAHPEAUlua_State@@H@Z",
                                 "hksi_lua_type");
        imported &= importSymbol(hModule, pushfstring,
                                 "?hksi_lua_pushfstring@@YAPEBDPEAUlua_State@@PEBDZZ",
                                 "hksi_lua_pushfstring");
        imported &= importSymbol(hModule, pushinteger,
                                 "?hksi_lua_pushinteger@@YAXPEAUlua_State@@H@Z",
                                 "hksi_lua_pushinteger");
        imported &= importSymbol(hModule, createtable,
                                 "?hksi_lua_createtable@@YAXPEAUlua_State@@HH@Z",
                                 "hksi_lua_createtable");
        imported &=
            importSymbol(hModule, dostring, "?DoString@LuaState@LuaPlus@@QEAAHPEBD@Z",
                         "LuaPlus::LuaState::DoString");
        imported &= importSymbol(hModule, pcall, "?lua_pcall@@YAHPEAUlua_State@@HHH@Z",
                                 "lua_pcall");
        imported &= importSymbol(hModule, loadbuffer,
                                 "?hksi_luaL_loadbuffer@@YAHPEAUlua_State@@PEBD_K1@Z",
                                 "hksi_luaL_loadbuffer");
        if (imported)
        {
            spdlog::debug("Imported all required Lua symbols");
        }
        return imported;
    }
    spdlog::error("Failed to find {}", Utils::wideToUtf8(LUA_DLL));
    return false;
}

struct lua_StatePartial
{
    char pad[0x48];
    struct TValue* top;
};

constexpr int LUA_TBOOLEAN = 1;

struct TValue
{
    int tt;
    int pad;
    uint64_t value;
};

void pushboolean(lua_State* L, bool value)
{
    auto* state = reinterpret_cast<lua_StatePartial*>(L);
    TValue* top = state->top;

    top->tt = LUA_TBOOLEAN;
    top->value = value ? 1ull : 0ull;

    state->top = top + 1;
}

bool isTable(lua_State* L, int index)
{
    return type(L, index) == LUA_TTABLE;
}

void pushstring(lua_State* L, const char* value)
{
    pushfstring(L, "%s", value ? value : "");
}
} // namespace LuaBindings
