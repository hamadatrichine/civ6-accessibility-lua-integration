#include "luaApi.h"
#include "luaMethods.h"
#include "luaAudio.h"

using namespace LuaBindings;
void LuaApi::injectAccessibilityTable(lua_State* L)
{
    createtable(L, 0, 13);
    pushClosure(L, LuaMethods::tolkOutput, -2, "Output");
    pushClosure(L, LuaMethods::tolkSilence, -2, "Silence");
    pushClosure(L, LuaMethods::tolkIsSpeaking, -2, "IsSpeaking");
    pushClosure(L, LuaMethods::tolkIsLoaded, -2, "IsLoaded");
    pushClosure(L, LuaMethods::tolkHasSpeech, -2, "HasSpeech");
    pushClosure(L, LuaMethods::tolkHasBraille, -2, "HasBraille");
    pushClosure(L, LuaMethods::tolkPreferSapi, -2, "PreferSapi");
    pushClosure(L, LuaMethods::tolkSpeak, -2, "Speak");
    pushClosure(L, LuaMethods::tolkBraille, -2, "Braille");
    pushClosure(L, LuaMethods::tolkDetectScreenReader, -2, "DetectScreenReader");
    pushClosure(L, LuaMethods::registerCharInputHandler, -2,
                "RegisterGlobalCharInputHandler");
    pushClosure(L, LuaMethods::unregisterCharInputHandler, -2,
                "UnregisterGlobalCharInputHandler");
    pushClosure(L, LuaMethods::getClipboardText, -2, "GetClipboardText");
    pushClosure(L, LuaMethods::getConfigValue, -2, "GetConfigValue");
    pushClosure(L, LuaMethods::setConfigValue, -2, "SetConfigValue");
    pushClosure(L, LuaMethods::isGameWindowFocused, -2, "IsGameWindowFocused");
    LuaAudio::registerApiFunctions(L);
    setfield(L, -2, "CAI");
}
