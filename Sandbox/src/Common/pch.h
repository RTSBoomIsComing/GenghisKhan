#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <limits> // std::numeric_limits<T>
#include <memory>
#include <iostream>