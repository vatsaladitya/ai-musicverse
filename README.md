# AI MUSICVERSE
**DISCLAIMER:** The abbreviation *AI* in the name of this project **DOES NOT MEAN ARTIFICIAL INTELLIGENCE!**. So, do not expect any AI stuff in this project.<br/>

This is a simple music player written in C++ and SDL2. This is the 2nd update to the original AI MUSICVERSE written in C++ with a bunch of other libraries. This project has completely migrated to SDL2 in this version. Previous libraries have been replaced with SDL2 libraries. For a complete list of changes, refer to [CHANGELOG](CHANGELOG.md).<br/>
Currently, it only supports Windows builds due to Win32 dependency. The lastest release can be found [here](https://github.com/vatsaladitya/ai-musicverse/releases/tag/v2.0.0).<br/>

# Controls
- <kbd>Arrow Up</kbd>/<kbd>Arrow Down</kbd> or Mouse Scroll Wheel -> Volume up/down
- <kbd>Arrow Right</kbd>/<kbd>Arrow Left</kbd> -> Music forward/back
- <kbd>Space</kbd> -> Music pause/play
- <kbd>D</kbd> -> Toggle light/dark mode
- <kbd>F</kbd> -> Toggle fullscreen

# Binaries
You can find all Windows binaries in the release section [here](https://github.com/vatsaladitya/ai-musicverse/releases).

# Build yourself
Dependencies:
1. [CMake](https://cmake.org/download)
2. [Visual Studio 2022](https://visualstudio.microsoft.com/vs) or [MinGW-w64](https://www.mingw-w64.org/downloads)
3. [SDL2](https://github.com/libsdl-org/SDL)
4. [SDL2_image](https://github.com/libsdl-org/SDL_image)
5. [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer)
6. [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)

This project uses CMake. A `CMakeLists.txt` file is provided in the root directory. The SDL2 libraries for Visual Studio 2022 x64 used to build this version and include files are provided along with the project for convenience. The following libraries are included:
- SDL2 v2.32.4
- SDL2_image v2.8.8
- SDL2_mixer v2.8.1
- SDL2_ttf v2.24.0

The .dll and .lib files are in **bin** and **lib** directories respectively. To build this project open command line interface at the project root directory and run the following commands:
```
mkdir build
cmake -S . -B build
cd build
cmake --build .
```
