# My OpenGL program
My OpenGL program is just me experimenting with OpenGL. <br>
The program uses GLFW, GLEW, IMGUI, and GLM. The project is inspired by The Chernos OpenGL youtube playlist,
but has major diffrences based on my preferences, and how i want to structure the code. Its a project i work on sometimes, with no real goals other than just learning.

## Build and run
It builds and runs on both windows and my linux (arch) machine.

#### Windows 
To build on windows for visual studio, first download premake5 <br>
in command promt run `winget install Premake.Premake.5.Beta` <br>
then to get the visual studio project files run `premake5 vs2022` (or whatever vs you use) in the project directory

#### Linux
For Linux it depends what distribution you use, but for arch its simple. <br>
first install premake run `sudo pacman -S premake5` <br>
then to create the project run `premake5 gmake` <br> 
finally to compile the code run `make -C Build/` <br>

## Key Binds
<b>Mouse1</b> to go into camera mode <br>
<b>Mouse</b> to move Camera <br>
<b>Esc</b> to leave camera mode, and if in window mode close program <br>
<b>WASD</b> to move, <b>C</b> to go down, <b>Spacebar</b> to go up <br>
<b>Q</b> and <b>E</b> to Roll camera <br>

## Preview
![Preview](https://i.gyazo.com/c748d074aba0e7100d40b2bf5c4e5f6e.gif)
