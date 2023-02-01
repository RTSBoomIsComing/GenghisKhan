#pragma once

#include "Keyboard.h"
#include "Mouse.h"

#include <Windows.h>
#include <cstdint>



namespace KhanApp
{
	class InputManager
	{
	public:
		Mouse mouse;
		Keyboard keyboard;
	};
}

