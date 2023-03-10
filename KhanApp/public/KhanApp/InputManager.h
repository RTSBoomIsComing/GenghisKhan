#pragma once

#include "Keyboard.h"
#include "Mouse.h"

#include <Windows.h>
#include <cstdint>
#include <entt/entt.hpp>


namespace KhanApp
{
	class InputManager
	{
		friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	public:
		Mouse mouse;
		Keyboard keyboard;
	};
}

