#pragma once
#include <memory>


namespace DirectX { // from DirectXTK, forward declaration.
	class Keyboard;	// Keyboard and Mouse would be replaced with my owns.
	class Mouse;
}

using std::unique_ptr;

namespace Khan {
	class Window;
	class Renderer;

	class Application
	{
	public:
		Application(unique_ptr<Window>& wnd);
		~Application() noexcept;
	public:
		int Run();
	protected:
		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<DirectX::Keyboard> m_keyboard;
		std::unique_ptr<DirectX::Mouse> m_mouse;

	private:
		void OnResizeWindow(UINT width, UINT height);
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}

