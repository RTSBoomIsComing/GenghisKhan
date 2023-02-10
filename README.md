# GenghisKhan
(This Project is in progress...)  
Collect Heroes and Conquer the world.  

## Framework
- Win32 API and directx 11 (d3d11)
- C++20 / MSBuild 
- vcpkg (So you might need microsoft`s visual studio)

## Dependencies
- directxmath
- entt
- stb
- assimp
- imgui

## vcpkg
All additional libraries (dependencies) are installed by vcpkg.  
It requires visual studio with vcpkg MSBuild integration.  
See the vcpkg.json file.  

Visual studio projects are set to use vcpkg manifest mode.
So you don't need to install libraries manually,
Once you build visual studio solution all the libraries installed automatically.

## Results of work so far
![Results](https://user-images.githubusercontent.com/61501369/218139932-9a4e0ba4-51d8-403a-bffd-e98bd0197abd.png)

## Done
- Input binding with keyboard and mouse
- Draw a selection rectangle with mouse 
- Camera movement with both mouse and keyboard
- Make Renderer allow instancing with either constant buffer or structured buffer (shader resource view)
- Import and render a 3d mesh(model) from file using Assimp library

## TODO
- add object-selecting(picking) function using selection rectangle or mouse click... -> need ray casting
- draw outlines of selected(picked) objects -> post processing, use of stencil buffer

