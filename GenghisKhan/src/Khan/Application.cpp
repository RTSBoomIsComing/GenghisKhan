#include "pch.h"
#include "Application.h"

#include "Common/Logger.h"
#include "Window/Window.h"
#include "Window/ImGuiManager.h"
#include "Renderer.h"

#include <directxtk/Keyboard.h>
#include <directxtk/Mouse.h>

using namespace Khan;

Application::Application(int window_width, int window_height, std::wstring window_name)
{
	m_window = std::make_unique<Window>(window_width, window_height, std::move(window_name));

	SetWindowLongPtr(m_window->GetHWnd(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLongPtr(m_window->GetHWnd(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));

	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(m_window->GetHWnd());

	m_renderer = std::make_unique<Renderer>(m_window->GetHWnd(),
		m_window->GetWidth(), m_window->GetHeight());

	ImGuiManager::Initialize(m_window->GetHWnd(), m_renderer->GetDevice().Get(), m_renderer->GetContext().Get());
}

Application::~Application()
{
	ImGuiManager::Destroy();
}

int Application::Run()
{
	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			GameLogic();

			ImGuiManager::Render();
			m_renderer->Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			m_renderer->Present();
		}
	}

	return static_cast<int>(msg.wParam);
}

void Application::OnResizeWindow(UINT width, UINT height)
{
	m_renderer->ResizeBackBuffers(width, height);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return 1u;

	switch (msg)
	{
	case WM_GETMINMAXINFO:
	{
		reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.x = 170;
		reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.y = 200;
		return 0u;
	}
	case WM_SIZE:
		if (wparam != SIZE_MINIMIZED)
		{
			auto app = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			app->OnResizeWindow((UINT)LOWORD(lparam), (UINT)HIWORD(lparam));
		}
		return 0u;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0u;
	case WM_ACTIVATEAPP:
		DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
		DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
		return 0u;
	case WM_MOUSEACTIVATE:
		// When you click activate the window, we want Mouse to ignore it.
		return MA_ACTIVATEANDEAT;
	case WM_ACTIVATE:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
		return 0u;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
		return 0u;
	case WM_SYSKEYDOWN:
		DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
		if (wparam == VK_RETURN && (lparam & 0x60000000) == 0x20000000)
		{
		}
		return 0u;	
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
