#include "luaMethods.h"
#include "config.h"
#include "charInput.h"
#include "luaBindings.h"
#include "luaTypes.h"
#include "utils.h"
#include <Tolk.h>

int LuaMethods::tolkOutput(LuaBindings::lua_State* L)
{
    const char* utf8Text = LuaBindings::checklstring(L, 1, nullptr);
    std::wstring wideText = Utils::utf8ToWide(utf8Text);
    bool interrupt = false;
    if (LuaBindings::gettop(L) >= 2)
    {
        interrupt = LuaBindings::toboolean(L, 2);
    }
    Tolk_Output(wideText.c_str(), interrupt);
    return 0;
}

int LuaMethods::tolkSpeak(LuaBindings::lua_State* L)
{
    const char* utf8Text = LuaBindings::checklstring(L, 1, nullptr);
    std::wstring wideText = Utils::utf8ToWide(utf8Text);
    bool interrupt = false;
    if (LuaBindings::gettop(L) >= 2)
    {
        interrupt = LuaBindings::toboolean(L, 2);
    }
    Tolk_Speak(wideText.c_str(), interrupt);
    return 0;
}

int LuaMethods::tolkSilence(LuaBindings::lua_State* L)
{
    Tolk_Silence();
    return 0;
}

int LuaMethods::tolkBraille(LuaBindings::lua_State* L)
{
    const char* utf8Text = LuaBindings::checklstring(L, 1, nullptr);
    std::wstring wideText = Utils::utf8ToWide(utf8Text);
    Tolk_Braille(wideText.c_str());
    return 0;
}

int LuaMethods::tolkIsSpeaking(LuaBindings::lua_State* L)
{
    bool isSpeaking = Tolk_IsSpeaking();
    LuaBindings::pushboolean(L, isSpeaking);
    return 1;
}

int LuaMethods::tolkIsLoaded(LuaBindings::lua_State* L)
{
    bool isLoaded = Tolk_IsLoaded();
    LuaBindings::pushboolean(L, isLoaded);
    return 1;
}

int LuaMethods::tolkHasSpeech(LuaBindings::lua_State* L)
{
    bool hasSpeech = Tolk_HasSpeech();
    LuaBindings::pushboolean(L, hasSpeech);
    return 1;
}

int LuaMethods::tolkHasBraille(LuaBindings::lua_State* L)
{
    bool hasBraille = Tolk_HasBraille();
    LuaBindings::pushboolean(L, hasBraille);
    return 1;
}

int LuaMethods::tolkPreferSapi(LuaBindings::lua_State* L)
{
    bool preferSapi = LuaBindings::toboolean(L, 1);
    Tolk_PreferSAPI(preferSapi);
    return 0;
}

int LuaMethods::tolkDetectScreenReader(LuaBindings::lua_State* L)
{
    const wchar_t* wideText = Tolk_DetectScreenReader();
    std::string utf8Text = Utils::wideToUtf8(wideText);
    LuaBindings::pushstring(L, utf8Text.c_str());
    return 1;
}

int LuaMethods::registerCharInputHandler(LuaBindings::lua_State* L)
{
    return CharInput::registerHandler(L);
}

int LuaMethods::unregisterCharInputHandler(LuaBindings::lua_State* L)
{
    return CharInput::unregisterHandler(L);
}

int LuaMethods::getClipboardText(LuaBindings::lua_State* L)
{
    std::string text = Utils::getClipboardText();
    LuaBindings::pushstring(L, text.c_str());
    return 1;
}

int LuaMethods::getConfigValue(LuaBindings::lua_State* L)
{
    const char* section = LuaBindings::checklstring(L, 1, nullptr);
    const char* key = LuaBindings::checklstring(L, 2, nullptr);
    const char* defaultValue = LuaBindings::checklstring(L, 3, nullptr);
    std::string value = Config::instance().getValue(section, key, defaultValue);
    LuaBindings::pushstring(L, value.c_str());
    return 1;
}

int LuaMethods::setConfigValue(LuaBindings::lua_State* L)
{
    const char* section = LuaBindings::checklstring(L, 1, nullptr);
    const char* key = LuaBindings::checklstring(L, 2, nullptr);
    const char* value = LuaBindings::checklstring(L, 3, nullptr);
    bool success = Config::instance().setValue(section, key, value);
    LuaBindings::pushboolean(L, success);
    return 1;
}
