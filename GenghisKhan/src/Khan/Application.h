#pragma once
#include <memory>


namespace DirectX { // from DirectXTK, forward declaration.
	class Keyboard;	// Keyboard and Mouse would be replaced with my owns.
	class Mouse;
}

namespace Khan {
	using pImgui = std::unique_ptr<class IImGui>;
	using pGame = std::unique_ptr<class IGame>;
	using pWnd = std::unique_ptr<class Window>;

	class Application
	{
	public:
		Application(pWnd& wnd, pGame& game, pImgui& imgui);
		~Application() noexcept;

		int Run();


	protected:
		std::unique_ptr<class IGame> m_game;
		std::unique_ptr<class IImGui> m_imgui;
		std::unique_ptr<class Window> m_window;

		std::unique_ptr<class Renderer> m_renderer;
		std::unique_ptr<DirectX::Keyboard> m_keyboard;
		std::unique_ptr<DirectX::Mouse> m_mouse;

	private:
		void OnResizeWindow(UINT width, UINT height);
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}

