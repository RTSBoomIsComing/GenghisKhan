#pragma once

// this pch is for Sandbox project

// win 32 api
#include "framework.h"
#include <wrl/client.h>
//#include <winuser.h>
using Microsoft::WRL::ComPtr;

// standard libraries
#include <limits> // std::numeric_limits<T>
#include <memory>
#include <iostream>
#include <format>
#include <vector>

// additional dependencies
#include <entt/entt.hpp>
#include <DirectXMath.h>
using DirectX::XMFLOAT4X4;
using DirectX::XMFLOAT3;

