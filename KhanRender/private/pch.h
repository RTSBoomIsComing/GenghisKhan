// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

// win 32 api
#include "framework.h"
#include <wrl.h>
using Microsoft::WRL::ComPtr;

// d3d11 dependencies
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>

// standard libraries
#include <format>
#include <vector>
#include <filesystem>
#include <cassert>
#include <chrono>
// additional dependencies
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#endif //PCH_H
