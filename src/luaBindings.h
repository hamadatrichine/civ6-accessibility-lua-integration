#pragma once
#include "luaTypes.h"

namespace LuaBindings
{
constexpr int GLOBALS_INDEX = -10002;
constexpr int REGISTRY_INDEX = -10000;
constexpr const wchar_t* LUA_DLL = L"HavokScript_FinalRelease.dll";

struct lua_State;

using cclosure_t = int(__cdecl*)(lua_State* L);
using pushnamedcclosure_t = void(__cdecl*)(lua_State* L, cclosure_t closure, int,
                                           const char* name, int);
extern pushnamedcclosure_t pushnamedcclosure;
using setfield_t = void(__cdecl*)(lua_State* L, int index, const char* key);
extern setfield_t setfield;
using getfield_t = void(__cdecl*)(lua_State* L, int index, const char* key);
extern getfield_t getfield;
using gettop_t = int(__cdecl*)(lua_State* L);
extern gettop_t gettop;
using pop_t = void(__thiscall*)(lua_State* L, int n);
extern pop_t pop;
using ref_t = int(__cdecl*)(lua_State* L, int index);
extern ref_t ref;
using unref_t = void(__cdecl*)(lua_State* L, int index, int ref);
extern unref_t unref;
using rawgeti_t = void(__cdecl*)(lua_State* L, int index, int n);
extern rawgeti_t rawgeti;
using checktype_t = void(__cdecl*)(lua_State* L, int index, LuaType t);
extern checktype_t checktype;
using checklstring_t = const char*(__cdecl*)(lua_State * L, int index, size_t* len);
extern checklstring_t checklstring;
using checkinteger_t = int(__cdecl*)(lua_State* L, int index);
extern checkinteger_t checkinteger;
using checknumber_t = double(__cdecl*)(lua_State* L, int index);
extern checknumber_t checknumber;
using pushnumber_t = void(__cdecl*)(lua_State* L, double n);
extern pushnumber_t pushnumber;
using pushnil_t = void(__cdecl*)(lua_State* L);
extern pushnil_t pushnil;
using toboolean_t = int(__cdecl*)(lua_State* L, int index);
extern toboolean_t toboolean;
using type_t = LuaType(__cdecl*)(lua_State* L, int index);
extern type_t type;
using pushfstring_t = const char*(__cdecl*)(lua_State * L, const char* fmt, ...);
extern pushfstring_t pushfstring;
using pushinteger_t = void(__cdecl*)(lua_State* L, int n);
extern pushinteger_t pushinteger;
using createtable_t = void(__cdecl*)(lua_State* L, int narr, int nrec);
extern createtable_t createtable;
using dostring_t = int(__thiscall*)(lua_State* L, const char* str);
extern dostring_t dostring;
using pcall_t = int(__cdecl*)(lua_State* L, int nargs, int nresults, int errfunc);
extern pcall_t pcall;
using loadbuffer_t = int(__cdecl*)(lua_State* L, const char* buff, size_t size,
                                   const char* name);
extern loadbuffer_t loadbuffer;

void pushClosure(lua_State* L, cclosure_t closure, int index, const char* key);
bool importSymbols();
bool isTable(lua_State* L, int index);
void pushboolean(lua_State* L, bool value);
void pushstring(lua_State* L, const char* value);
} // namespace LuaBindings
