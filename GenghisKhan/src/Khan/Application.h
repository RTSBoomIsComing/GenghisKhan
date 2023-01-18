#pragma once
#include <memory>

namespace DirectX	// from DirectXTK, forward declaration
{
	class Keyboard;
	class Mouse;
}

namespace Khan {
	class Application
	{
	public:
		Application(int window_width, int window_height, std::wstring window_name);
		~Application();

		void Update();
		int Run();
	private:
		void Render();
		void OnResizeWindow(UINT width, UINT height);
	private:
		std::unique_ptr<class Window> m_window;
		std::unique_ptr<class Renderer> m_renderer;
		std::unique_ptr<DirectX::Keyboard> m_keyboard;
		std::unique_ptr<DirectX::Mouse> m_mouse;


		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}

