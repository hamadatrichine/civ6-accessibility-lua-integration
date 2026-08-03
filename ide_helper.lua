---@meta

---@alias str_utf8 string
---@alias SoundHandle integer

---@alias AttenuationModel
---| 0 # None
---| 1 # Inverse
---| 2 # Linear
---| 3 # Exponential

---@class CAI
local CAI = {}

---@class CAIAttenuationModel
---@field None 0
---@field Inverse 1
---@field Linear 2
---@field Exponential 3

---@type CAIAttenuationModel
CAI.AttenuationModel = {}

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

---@return boolean
function CAI.IsGameWindowFocused() end

---@return str_utf8
function CAI.GetLatestVersion() end

--------------------------------------------------------------------------------
-- Audio
--------------------------------------------------------------------------------

---@param filePath str_utf8
---@return SoundHandle|nil
function CAI.LoadSound(filePath) end

---@param handle SoundHandle
---@return boolean
function CAI.DestroySound(handle) end

---@param handle SoundHandle
function CAI.PlaySound(handle) end

---@param handle SoundHandle
function CAI.PauseSound(handle) end

---@param handle SoundHandle
function CAI.StopSound(handle) end

---@param handle SoundHandle
---@param volume number # 0.0 = silent, 1.0 = normal
function CAI.SetSoundVolume(handle, volume) end

---@param handle SoundHandle
---@return number
function CAI.GetSoundVolume(handle) end

---@param volume number # 0.0 = silent, 1.0 = normal
function CAI.SetMasterVolume(volume) end

---@return number
function CAI.GetMasterVolume() end

---@param handle SoundHandle
---@param looping boolean
function CAI.SetSoundLooping(handle, looping) end

---@param handle SoundHandle
---@return boolean
function CAI.IsSoundLooping(handle) end

---@param handle SoundHandle
---@return boolean
function CAI.IsSoundPlaying(handle) end

---@param handle SoundHandle
---@param pitch number # 1.0 = normal
function CAI.SetSoundPitch(handle, pitch) end

---@param handle SoundHandle
---@return number
function CAI.GetSoundPitch(handle) end

---@param handle SoundHandle
---@param pan number # -1.0 = left, 0.0 = center, 1.0 = right
function CAI.SetSoundPan(handle, pan) end

---@param handle SoundHandle
---@return number
function CAI.GetSoundPan(handle) end

---@param handle SoundHandle
---@param x number
---@param y number
---@param z number
function CAI.SetSoundPosition(handle, x, y, z) end

---@param handle SoundHandle
---@return number x
---@return number y
---@return number z
function CAI.GetSoundPosition(handle) end

---@param handle SoundHandle
---@param x number
---@param y number
---@param z number
function CAI.SetSoundDirection(handle, x, y, z) end

---@param handle SoundHandle
---@param x number
---@param y number
---@param z number
function CAI.SetSoundVelocity(handle, x, y, z) end

---@param handle SoundHandle
---@param enabled boolean
function CAI.SetSoundSpatializationEnabled(handle, enabled) end

---@param handle SoundHandle
---@return boolean
function CAI.IsSoundSpatializationEnabled(handle) end

---@param handle SoundHandle
---@param distance number
function CAI.SetSoundMinDistance(handle, distance) end

---@param handle SoundHandle
---@param distance number
function CAI.SetSoundMaxDistance(handle, distance) end

---@param handle SoundHandle
---@param model AttenuationModel
function CAI.SetSoundAttenuationModel(handle, model) end

---@param x number
---@param y number
---@param z number
function CAI.SetListenerPosition(x, y, z) end

---@param x number
---@param y number
---@param z number
function CAI.SetListenerDirection(x, y, z) end

---@param x number
---@param y number
---@param z number
function CAI.SetListenerUp(x, y, z) end

---@param x number
---@param y number
---@param z number
function CAI.SetListenerVelocity(x, y, z) end

function CAI.AudioUpdate() end

---@class ExposedMembers
---@field CAI CAI
ExposedMembers = ExposedMembers or {}
ExposedMembers.CAI = CAI
