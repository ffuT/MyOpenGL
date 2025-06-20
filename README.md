# MyOpenGL

This is my personal OpenGL playground. The project is inspired by TheCherno’s OpenGL YouTube playlist, but I’ve made lots of changes based on my own preferences and learning goals. There’s no grand vision, im just experimenting, learning, and having fun with graphics programming.

## Features
- Uses GLFW, GLEW, ImGui, and GLM
- Cross-platform: builds and runs on both Windows and Linux (tested on Arch)
- Camera controls
- Scene editor using ImGui
- Supports loading `.obj` meshes

## Build and Run
### Windows
1. Download [Premake5](https://premake.github.io/) <- there, or open a command prompt and install:
   ```
   winget install Premake.Premake.5.Beta
   ```
2. Generate Visual Studio project files (replace `vs2022` with your version):
   ```
   premake5 vs2022
   ```
3. Open the solution in Visual Studio and build.
### Linux (Arch)
1. Install dependencies:
   ```
   sudo pacman -S glew glfw premake
   ```
2. Generate makefiles:
   ```
   premake5 gmake
   ```
3. Build the project:
   ```
   make -C Build/ config=release_x64
   ```
## Controls
- **Mouse 1:** Enter camera mode
- **Mouse:** Move camera
- **Esc:** Exit camera mode / close program (if in window mode)
- **WASD:** Move
- **C:** Move down
- **Spacebar:** Move up
- **Q/E:** Roll camera
  
## Project Structure
- `src/` — C++ source code
- `res/` — Resources (shaders, models, textures)
- `premake5.lua` — Project configuration
  
## Notes
- This repo is a work-in-progress and changes often.
- Any weird code/comments is likely just me experimenting or leaving notes to myself.
  
## Preview (old)
![Preview](https://i.gyazo.com/c748d074aba0e7100d40b2bf5c4e5f6e.gif)
