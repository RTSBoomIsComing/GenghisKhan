#pragma once

// this pch is for Sandbox project

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <limits> // std::numeric_limits<T>
#include <memory>
#include <iostream>
#include <format>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <entt/entt.hpp>

#include <DirectXMath.h>
