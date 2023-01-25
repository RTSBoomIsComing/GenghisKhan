#pragma once

//#include <directxtk/Keyboard.h>
//#include <directxtk/Mouse.h>
#include "Window/Window.h"
#include <directxtk/Keyboard.h>
#include <directxtk/Mouse.h>
#include <memory>
using std::unique_ptr;


namespace Khan {

	class Application abstract : public Window
	{
	public:
		Application(int width = 1024, int height = 768, std::wstring name = L"genghis khan");
		virtual ~Application() noexcept;
	
		virtual void ImGuiRender_Impl() abstract;
		int Run();

	protected:
		virtual void Update() abstract;
		virtual void Render() abstract;
	
	private:
		void OnResizeWindow(UINT width, UINT height);

	protected:
		unique_ptr<DirectX::Keyboard> m_keyboard;
		unique_ptr<DirectX::Mouse> m_mouse;

	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}

