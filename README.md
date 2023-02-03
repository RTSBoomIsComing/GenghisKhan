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