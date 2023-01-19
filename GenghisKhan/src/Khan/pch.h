#pragma once

#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <limits> // std::numeric_limits<T>
#include <memory>
#include <iostream>