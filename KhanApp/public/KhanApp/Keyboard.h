#pragma once
#include "Delegate.h"
#include <bitset>
namespace KhanApp
{
	class Keyboard
	{
		friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	public:
		// Return types of funcions bound to Delegate are void
		Delegate<unsigned char, int> OnKeyDown{}; 
		Delegate<unsigned char, int> OnKeyUp{}; 

	private:
		//void ProcessMessage(uint32_t keyCode, bool wasDown, bool isDown) noexcept;
		std::bitset<256ui64> keyState;
	};
}

