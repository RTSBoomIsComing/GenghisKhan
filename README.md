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
![Results](https://user-images.githubusercontent.com/61501369/217764200-53519382-2534-4180-a96c-7fa8dcd398c6.png)

## TODO
- make CubeRenderer allow instancing
- make Cube entity with entt and test rendering a lot of cubes with instancing
- add feature selecting(picking) objects by mouse
- draw outlines of selected(picked) objects
- Keyboard event system
- load and render 3d model from file using assimp library

