#pragma once
#include "Delegate.h"
#include <bitset>
#include <map>

#include <functional>

namespace KhanApp
{
	class Keyboard
	{
		friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	public:
		std::bitset<256> KeyStates;

		std::unordered_map<unsigned char, Delegate<>> OnKeyDown;
		std::unordered_map<unsigned char, Delegate<>> OnKeyUp;
		
	};
}

