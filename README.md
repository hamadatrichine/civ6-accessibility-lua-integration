# Civ VI Accessibility Lua Integration

[![Build and Release](https://github.com/hamadatrichine/civ6-accessibility-lua-integration/actions/workflows/release.yml/badge.svg)](https://github.com/hamadatrichine/civ6-accessibility-lua-integration/actions/workflows/release.yml)

DLL integration layer that exposes selected Tolk screen-reader functions to
Civilization VI's Lua environment in addition to character input routing, configuration, audio, and
clipboard functionality.

## Requirements

- 64-bit Windows
- CMake
- A C/C++ compiler supported by CMake, tested with MSVC 2026
- Sid Meier's Civilization VI, tested with the steam version

## Build

Configure once:

```powershell
cmake --preset windows
```

Release build:

```powershell
cmake --build --preset release
```

Debug build:

```powershell
cmake --build --preset debug
```

## Build Outputs

- Release: `build\Release\LightFX.dll`
- Debug: `build\Debug\LightFX.dll`

## Usage

After building, copy the runtime DLLs from the build output directory to the
Civilization VI executable directory.

For the Steam version, the target directory is usually:

```text
C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization VI\Base\Binaries\Win64Steam
```

For a normal release build, copy:

```text
build\Release\LightFX.dll
build\Release\nvdaControllerClient64.dll
build\Release\SAAPI64.dll
```

For a development build with console/debug logging, copy the same filenames from
`build\Debug\` instead.

The DLL must be named `LightFX.dll` in the game directory. Civilization VI loads
that filename through its LightFX integration point.

Runtime logs are written to:

```text
%LOCALAPPDATA%\Firaxis Games\Sid Meier's Civilization VI\Logs\civ6-accessibility-integration.log
```

## Lua API

`ide_helper.lua` contains LuaLS/EmmyLua annotations for `ExposedMembers.CAI`.

The DLL injects the following functions:

```lua
-- Screen reader
ExposedMembers.CAI.Output(text: str_utf8, interrupt: bool) -> nil
ExposedMembers.CAI.Speak(text: str_utf8, interrupt: bool) -> nil
ExposedMembers.CAI.Silence() -> nil
ExposedMembers.CAI.Braille(text: str_utf8) -> nil
ExposedMembers.CAI.IsSpeaking() -> bool
ExposedMembers.CAI.IsLoaded() -> bool
ExposedMembers.CAI.HasSpeech() -> bool
ExposedMembers.CAI.HasBraille() -> bool
ExposedMembers.CAI.PreferSapi(prefer: bool) -> nil
ExposedMembers.CAI.DetectScreenReader() -> str_utf8

-- Utilities
ExposedMembers.CAI.GetClipboardText() -> str_utf8
ExposedMembers.CAI.RegisterGlobalCharInputHandler(callback: (char: str_utf8) -> nil) -> nil
ExposedMembers.CAI.UnregisterGlobalCharInputHandler() -> nil
ExposedMembers.CAI.GetConfigValue(section: str_utf8, key: str_utf8, defaultValue: str_utf8) -> str_utf8
ExposedMembers.CAI.SetConfigValue(section: str_utf8, key: str_utf8, value: str_utf8) -> bool
ExposedMembers.CAI.IsGameWindowFocused() -> bool
ExposedMembers.CAI.GetLatestVersion() -> str_utf8

-- Audio
ExposedMembers.CAI.LoadSound(filePath: str_utf8) -> SoundHandle | nil
ExposedMembers.CAI.DestroySound(handle: SoundHandle) -> bool

ExposedMembers.CAI.PlaySound(handle: SoundHandle) -> nil
ExposedMembers.CAI.PauseSound(handle: SoundHandle) -> nil
ExposedMembers.CAI.StopSound(handle: SoundHandle) -> nil

ExposedMembers.CAI.SetSoundVolume(handle: SoundHandle, volume: number) -> nil
ExposedMembers.CAI.GetSoundVolume(handle: SoundHandle) -> number

ExposedMembers.CAI.SetMasterVolume(volume: number) -> nil
ExposedMembers.CAI.GetMasterVolume() -> number

ExposedMembers.CAI.SetSoundLooping(handle: SoundHandle, looping: bool) -> nil
ExposedMembers.CAI.IsSoundLooping(handle: SoundHandle) -> bool

ExposedMembers.CAI.SetSoundPitch(handle: SoundHandle, pitch: number) -> nil
ExposedMembers.CAI.GetSoundPitch(handle: SoundHandle) -> number

ExposedMembers.CAI.SetSoundPan(handle: SoundHandle, pan: number) -> nil
ExposedMembers.CAI.GetSoundPan(handle: SoundHandle) -> number

ExposedMembers.CAI.SetSoundPosition(handle: SoundHandle, x: number, y: number, z: number) -> nil
ExposedMembers.CAI.GetSoundPosition(handle: SoundHandle) -> (number, number, number)

ExposedMembers.CAI.SetSoundDirection(handle: SoundHandle, x: number, y: number, z: number) -> nil
ExposedMembers.CAI.SetSoundVelocity(handle: SoundHandle, x: number, y: number, z: number) -> nil

ExposedMembers.CAI.SetSoundSpatializationEnabled(handle: SoundHandle, enabled: bool) -> nil
ExposedMembers.CAI.IsSoundSpatializationEnabled(handle: SoundHandle) -> bool

ExposedMembers.CAI.SetSoundMinDistance(handle: SoundHandle, distance: number) -> nil
ExposedMembers.CAI.SetSoundMaxDistance(handle: SoundHandle, distance: number) -> nil

ExposedMembers.CAI.SetSoundAttenuationModel(handle: SoundHandle, model: AttenuationModel) -> nil

ExposedMembers.CAI.IsSoundPlaying(handle: SoundHandle) -> bool

-- Listener
ExposedMembers.CAI.SetListenerPosition(x: number, y: number, z: number) -> nil
ExposedMembers.CAI.SetListenerDirection(x: number, y: number, z: number) -> nil
ExposedMembers.CAI.SetListenerUp(x: number, y: number, z: number) -> nil
ExposedMembers.CAI.SetListenerVelocity(x: number, y: number, z: number) -> nil
ExposedMembers.CAI.AudioUpdate() -> nil
```

### AttenuationModel

`SetSoundAttenuationModel()` accepts one of the following values:

| Value | Name          |
| ----: | ------------- |
|   `0` | `None`        |
|   `1` | `Inverse`     |
|   `2` | `Linear`      |
|   `3` | `Exponential` |

When using the provided `ide_helper.lua`, these are also available as:

```lua
CAI.AttenuationModel.None
CAI.AttenuationModel.Inverse
CAI.AttenuationModel.Linear
CAI.AttenuationModel.Exponential
```
