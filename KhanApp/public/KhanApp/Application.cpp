#include "pch.h"
#include "Application.h"
#include <KhanTools/Log.h>
#include <Windowsx.h>
#include "imgui_impl_win32.h"

LRESULT CALLBACK KhanApp::WndProc(HWND, UINT, WPARAM, LPARAM);

KhanApp::Application::Application()
	:
	Window()
{
	::SetWindowLongPtrW(m_window_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	::SetWindowLongPtrW(m_window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));
}

KhanApp::Application::~Application() noexcept
{
}

void KhanApp::Application::OnResizeWindow(UINT width, UINT height) noexcept
{
	m_window_width = width;
	m_window_height = height;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace KhanApp 
{
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			return 1u;
		auto app = reinterpret_cast<KhanApp::Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		assert(app && "app is nullptr");

		switch (msg)
		{
		case WM_GETMINMAXINFO:
			reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.x = 170;
			reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.y = 200;
			return 0u;

		case WM_SIZE:
			if (wparam != SIZE_MINIMIZED)
			{
				app->OnResizeWindow((UINT)LOWORD(lparam), (UINT)HIWORD(lparam));
			}
			return 0u;

		case WM_CLOSE:
			::PostQuitMessage(0);
			return 0u;
		case WM_KILLFOCUS:	// I think it would be better than using WM_ACTIVATE
			app->DisableMouseLockToWindow();
			return 0u;

		//case WM_SETFOCUS:
//		case WM_MOUSEACTIVATE:
//		{
//			POINT Pos{};
//			::GetCursorPos(&Pos);
//			RECT clientRect{};
//			GetClientRect(hwnd, &clientRect);
//			if (::PtInRect(&clientRect, Pos))
//			{
//				app->EnableMouseLockToWindow(true);
//			}
//			return MA_ACTIVATE;
//		}
		//case WM_ACTIVATE:
		//case WM_INPUT:
		case WM_MOUSEMOVE:
		{
			const POINTS pos = MAKEPOINTS(lparam);
			app->m_input.mouse.OnMouseMove(pos.x, pos.y);
			return 0u;
		}
		case WM_LBUTTONDOWN:
		{
			app->EnableMouseLockToWindow();

			const POINT pos{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
			app->m_input.mouse.OnLeftButtonDown(pos.x, pos.y);
			return 0u;
		}
		case WM_LBUTTONUP:
		{
			const POINT pos{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
			app->m_input.mouse.OnLeftButtonUp(pos.x, pos.y);
			return 0u;
		}
		case WM_RBUTTONDOWN:
		{
			app->EnableMouseLockToWindow();

			const POINT pos{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
			app->m_input.mouse.OnRightButtonDown(pos.x, pos.y);
			return 0u;
		}
		case WM_RBUTTONUP:
		{
			const POINT Pos{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
			app->m_input.mouse.OnRightButtonUp(Pos.x, Pos.y);
			return 0u;
		}
		//case WM_MBUTTONDOWN:
		//case WM_MBUTTONUP:
		//case WM_MOUSEWHEEL:
		//case WM_XBUTTONDOWN:
		//case WM_XBUTTONUP:
		//case WM_MOUSEHOVER:
		//	//DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
		//	//return 0u;
		//case WM_KEYDOWN:
		//case WM_KEYUP:
		//case WM_SYSKEYUP:
		//case WM_SYSKEYDOWN:
		//{
		//	uint32_t VKCode = LOWORD(wparam);

		//	bool wasDown = (lparam & 0x4000'0000u) != 0;
		//	bool isDown = (lparam & 0x8000'0000u) == 0;
		//	//app->m_input.keyboard.ProcessMessage(VKCode, wasDown, isDown);
		//}
		//DirectX::Keyboard::ProcessMessage(msg, wparam, lparam);
		//if (wparam == VK_RETURN && (lparam & 0x60000000) == 0x20000000)
		//{
		//}
		//return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}