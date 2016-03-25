# GL Framework

A 3D OpenGL framework thingy. Hopefully usable for future projects.

## TODO:
- Use SDL2 as much as possible for maximum cross platform
	- `SDL_Log` instead of `std::cout`
	- `SDL_RWops` instead of `fstream`
	- etc.
- bitmap font renderer
	- have a default font in code?
- image cache, multiple file types, at least .png + .bmp
	- key colours for transparency
- audio cache,
- timer class, gives game ticks and delta time
- 3D camera
- resizeable window
- fixed step 3D physics, start with AABBs
- splash screen + menu system
- save / load level data from text file
- save / load player data from text fle
- save / load options from text file, resolution, fullscreen/windowed, sensitivity, volume
- change keybindings
- controller support
- learn how to bundle as .app
- compile with OpenGL or OpenGL ES
- lightmaps
- radiosity?
- scripting?