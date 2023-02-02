#pragma once
#include "Window.h"
#include "InputManager.h"

namespace KhanApp
{
	class Application : public Window
	{
	public:
		Application();
		virtual ~Application() noexcept;

		// when override this function, have to call this function in overrided function.
		virtual void OnResizeWindow(UINT width, UINT height) noexcept;

		InputManager m_input;
	};
}

