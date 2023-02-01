#pragma once
#include "Window.h"
#include "InputManager.h"

//extern LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace KhanApp
{
	class Application : public Window
	{
	public:
		Application();
		virtual ~Application() noexcept;
		void OnResizeWindow(UINT width, UINT height) noexcept;

		InputManager m_input;
	};
}

