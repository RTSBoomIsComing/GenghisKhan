#pragma once
#include <memory>

#include <directxtk/Keyboard.h>
#include <directxtk/Mouse.h>
#include "Window/Window.h"
#include "Renderer.h"

//namespace DirectX { // from DirectXTK, forward declaration.
//	class Keyboard;	// Keyboard and Mouse would be replaced with my owns.
//	class Mouse;
//}

using std::unique_ptr;

namespace Khan {
	//class Window;
	//class Renderer;

	class Application abstract
	{
	public:
		Application(int width = 1024, int height = 768, std::wstring name = L"genghis khan");
		virtual ~Application() noexcept = default;
	public:
		int Run();
		virtual void Logic() abstract;
		virtual void Render() abstract;
		virtual void ImGuiRender_Impl() abstract;
	protected:
		unique_ptr<Window> m_window;
		unique_ptr<Renderer> m_renderer;
		unique_ptr<DirectX::Keyboard> m_keyboard;
		unique_ptr<DirectX::Mouse> m_mouse;

	private:
		void OnResizeWindow(UINT width, UINT height);
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}

