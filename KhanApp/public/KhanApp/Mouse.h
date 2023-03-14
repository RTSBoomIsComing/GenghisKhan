#pragma once
#include "Delegate.h"
#include <bitset>

namespace KhanApp
{
	class Mouse
	{
		friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		friend class InputManager;
	public:
		// Return types of funcions bound to Delegate are void
		Delegate<int, int> OnLeftButtonDown{};
		Delegate<int, int> OnLeftButtonUp{};

		Delegate<int, int> OnRightButtonDown{};
		Delegate<int, int> OnRightButtonUp{};

		Delegate<int, int> OnMouseMove{};
		Delegate<int, int> OnMouseRawInput{};

		Delegate<int, int, int> OnMouseWheel{};
	};
}
