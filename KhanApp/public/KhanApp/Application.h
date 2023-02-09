#pragma once
#include "Window.h"
#include "InputManager.h"

namespace KhanApp
{
	class Application : public Window
	{
		friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	public:
		Application();
		virtual ~Application() noexcept;

		// when override this function, have to call this function in overrided function.
		virtual void OnResizeWindow(UINT width, UINT height) noexcept;

	public:
		float m_aspectRatio{ float(m_window_width) / m_window_height };
		InputManager m_input;
	};
}

