#include "luaUpdateManager.h"
#include "updateManager.h"

using namespace LuaBindings;
namespace
{
static int luaGetLatestVersion(lua_State* L)
{
    auto& updateManager = UpdateManager::instance();
    auto version = updateManager.getLatestVersion();
    pushstring(L, version.c_str());
    return 1;
}
} // anonymous namespace

void LuaUpdateManager::registerApiFunctions(LuaBindings::lua_State* L)
{
    pushClosure(L, luaGetLatestVersion, -2, "GetLatestVersion");
}
