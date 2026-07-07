---@meta

---@alias str_utf8 string

---@class CAI
local CAI = {}

---@param text str_utf8
---@param interrupt boolean
function CAI.Output(text, interrupt) end

---@param text str_utf8
---@param interrupt boolean
function CAI.Speak(text, interrupt) end

function CAI.Silence() end

---@param text str_utf8
function CAI.Braille(text) end

---@return boolean
function CAI.IsSpeaking() end

---@return boolean
function CAI.IsLoaded() end

---@return boolean
function CAI.HasSpeech() end

---@return boolean
function CAI.HasBraille() end

---@param prefer boolean
function CAI.PreferSapi(prefer) end

---@return str_utf8
function CAI.DetectScreenReader() end

---@return str_utf8
function CAI.GetClipboardText() end

---@param callback fun(char: str_utf8)
function CAI.RegisterGlobalCharInputHandler(callback) end

function CAI.UnregisterGlobalCharInputHandler() end

---@param section str_utf8
---@param key str_utf8
---@param defaultValue str_utf8
---@return str_utf8
function CAI.GetConfigValue(section, key, defaultValue) end

---@param section str_utf8
---@param key str_utf8
---@param value str_utf8
---@return boolean
function CAI.SetConfigValue(section, key, value) end

---@class ExposedMembers
---@field CAI CAI
ExposedMembers = ExposedMembers or {}
ExposedMembers.CAI = CAI
