#include "charInput.h"
#include "luaTypes.h"
#include <spdlog/spdlog.h>

namespace
{
struct Handler
{
    LuaBindings::lua_State* state = nullptr;
    int ref = -2;
};

Handler handler;

void clearHandler()
{
    handler.state = nullptr;
    handler.ref = -2;
}
} // namespace

void CharInput::callHandler(const char* utf8Text)
{
    LuaBindings::lua_State* L = handler.state;
    int ref = handler.ref;
    if (!L || ref <= 0)
    {
        spdlog::debug("Dropped char input because no Lua handler is registered");
        clearHandler();
        return;
    }

    LuaBindings::rawgeti(L, LuaBindings::REGISTRY_INDEX, ref);
    LuaBindings::pushstring(L, utf8Text);
    if (LuaBindings::pcall(L, 1, 0, 0) != 0)
    {
        const char* err = LuaBindings::checklstring(L, -1, nullptr);
        spdlog::error("Lua error: {}", err);
        LuaBindings::pop(L, 1);
        clearHandler();
    }
}

int CharInput::registerHandler(LuaBindings::lua_State* L)
{
    unregisterHandler(L);
    LuaBindings::checktype(L, 1, LUA_TFUNCTION);
    int ref = LuaBindings::ref(L, LuaBindings::REGISTRY_INDEX);
    if (ref > 0)
    {
        handler.state = L;
        handler.ref = ref;
        spdlog::debug("Registered char input handler ref {} in Lua registry", ref);
    }
    return 0;
}

int CharInput::unregisterHandler(LuaBindings::lua_State* L)
{
    if (handler.ref > 0)
    {
        spdlog::debug("Unregistered char input handler ref {}", handler.ref);
        LuaBindings::unref(L, LuaBindings::REGISTRY_INDEX, handler.ref);
    }
    clearHandler();
    return 0;
}
