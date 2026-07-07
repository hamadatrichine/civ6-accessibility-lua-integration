# Civ VI Accessibility Integration

DLL integration layer that exposes selected Tolk screen-reader functions to
Civilization VI's Lua environment in addition to character input routing, configuration, and
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
ExposedMembers.CAI.GetClipboardText() -> str_utf8
ExposedMembers.CAI.RegisterGlobalCharInputHandler(callback: (char: str_utf8) -> nil) -> nil
ExposedMembers.CAI.UnregisterGlobalCharInputHandler() -> nil
ExposedMembers.CAI.GetConfigValue(section: str_utf8,  key: str_utf8, defaultValue: str_utf8) -> str_utf8
ExposedMembers.CAI.SetConfigValue(section: str_utf8,  key: str_utf8, value: str_utf8) -> bool
```
