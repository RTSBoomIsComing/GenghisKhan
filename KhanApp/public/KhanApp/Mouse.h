#pragma once
#include "Delegate.h"
namespace KhanApp
{
	class Mouse
	{
		friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	public:
		// Return types of funcions bound to Delegate are void
		Delegate<int, int> OnLeftButtonDown{};
		Delegate<int, int> OnLeftButtonUp{};

		Delegate<int, int> OnRightButtonDown{};
		Delegate<int, int> OnRightButtonUp{};
		
		Delegate<int, int> OnMouseMove{};
	};
}
