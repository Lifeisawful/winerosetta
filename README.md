# Winerosetta Library
[![Build](https://github.com/Lifeisawful/winerosetta/actions/workflows/build.yml/badge.svg)](https://github.com/Lifeisawful/winerosetta/actions/workflows/build.yml)

Winerosetta is a library specifically designed for Apple Silicon, WINE, and the 32-bit versions of World of Warcraft (1.12.1, 2.4.3 and 3.3.5a). It works around the limitations of Rosetta 2 by providing support for certain instructions not natively supported.

## Installation

For Turtle-WoW:

1. Download the `winerosetta.dll` from the latest release.
2. Navigate to your Turtle-WoW installation directory.
3. Replace the existing `DiscordOverlay.dll` with the downloaded `winerosetta.dll`.

1.12.1 / 2.4.3 / 3.3.5a:

1. Download the `winerosetta.dll` and `winerosettaldr.exe` from the latest release.
2. Navigate to your WoW installation directory.
3. Copy `winerosetta.dll` and `winerosettaldr.exe` here
4. Use `winerosettaldr.exe` to play.

## Optional OpenGL Configuration (1.12.1)

To get OpenGL working, you need to modify the `Config.WTF` file with the following settings:

```
SET gxApi "OpenGl"
SET gxColorBits "32"
SET gxDepthBits "32"
```

Then, set the environment variable ```GL_NV_vertex_program``` to ```0```.

Please note that from experience, Direct3D (d3d) tends to work better on M1 Mac.

## License

This project is licensed under the terms of the MIT license. 