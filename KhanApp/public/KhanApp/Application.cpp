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

	// Register the mouse raw input device
	RAWINPUTDEVICE rid{};
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = m_window_handle;
	if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
	{
		KHAN_ERROR("Failed to register raw input device");
		throw std::exception("Failed to register raw input device");
	}
}

KhanApp::Application::~Application() noexcept
{
}

void KhanApp::Application::OnResizeWindow(UINT width, UINT height) noexcept
{
	m_window_width = width;
	m_window_height = height;
	m_aspectRatio = float(width) / height;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace KhanApp
{
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			return 1;

		auto app = reinterpret_cast<KhanApp::Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		assert(app && "app is nullptr");

		switch (msg)
		{
		case WM_INPUT:
		{
			UINT dwSize = sizeof(RAWINPUT);
			static BYTE lpb[sizeof(RAWINPUT)];

			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				LONG xPosRelative = raw->data.mouse.lLastX;
				LONG yPosRelative = raw->data.mouse.lLastY;
				app->m_input.mouse.OnMouseRawInput(xPosRelative, yPosRelative);
			}
			return 0;
		}

		case WM_GETMINMAXINFO:
			reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.x = 170;
			reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.y = 200;
			return 0;

		case WM_SIZE:
			if (wparam != SIZE_MINIMIZED)
			{
				app->OnResizeWindow((UINT)LOWORD(lparam), (UINT)HIWORD(lparam));
			}
			return 0;

		case WM_CLOSE:
			::PostQuitMessage(0);
			return 0;
		case WM_KILLFOCUS:	// I think it would be better than using WM_ACTIVATE
			app->DisableMouseLockToWindow();
			app->m_input.mouse.ButtonStates.reset();
			app->m_input.keyboard.KeyStates.reset();
			while (::ShowCursor(true) < 0);
			return 0;

		case WM_MOUSEMOVE:
		{
			const POINTS pos = MAKEPOINTS(lparam);
			app->m_input.mouse.Positions[static_cast<int>(MouseEvent::MOVE)] = pos;
			app->m_input.mouse.OnMouseMove(pos.x, pos.y);

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			app->EnableMouseLockToWindow();
			if (ImGui::GetIO().WantCaptureMouse) return 192U;

			app->m_input.mouse.ButtonStates[static_cast<int>(Mouse::ButtonType::LEFT)] = true;

			const POINTS pos = MAKEPOINTS(lparam);
			app->m_input.mouse.Positions[static_cast<int>(MouseEvent::LEFT_DOWN)] = pos;
			app->m_input.mouse.OnLeftButtonDown(pos.x, pos.y);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			if (ImGui::GetIO().WantCaptureMouse) return 192U;
			app->m_input.mouse.ButtonStates[static_cast<int>(Mouse::ButtonType::LEFT)] = false;

			const POINTS pos = MAKEPOINTS(lparam);
			app->m_input.mouse.Positions[static_cast<int>(MouseEvent::LEFT_UP)] = pos;
			app->m_input.mouse.OnLeftButtonUp(pos.x, pos.y);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			app->EnableMouseLockToWindow();
			if (ImGui::GetIO().WantCaptureMouse) return 192U;

			app->m_input.mouse.ButtonStates[static_cast<int>(Mouse::ButtonType::RIGHT)] = true;

			const POINTS pos = MAKEPOINTS(lparam);
			app->m_input.mouse.Positions[static_cast<int>(MouseEvent::RIGHT_DOWN)] = pos;
			app->m_input.mouse.OnRightButtonDown(pos.x, pos.y);
			return 0u;
		}
		case WM_RBUTTONUP:
		{
			if (ImGui::GetIO().WantCaptureMouse) return 192U;
			app->m_input.mouse.ButtonStates[static_cast<int>(Mouse::ButtonType::RIGHT)] = false;

			const POINTS pos = MAKEPOINTS(lparam);
			app->m_input.mouse.Positions[static_cast<int>(MouseEvent::RIGHT_UP)] = pos;
			app->m_input.mouse.OnRightButtonUp(pos.x, pos.y);
			return 0;
		}
		//case WM_MBUTTONDOWN:
		//case WM_MBUTTONUP:
		//case WM_MOUSEWHEEL:
		//case WM_XBUTTONDOWN:
		//case WM_XBUTTONUP:
		//case WM_MOUSEHOVER:
		//	//DirectX::Mouse::ProcessMessage(msg, wparam, lparam);
		//	//return 0u;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			bool wasDown = (lparam & 0x4000'0000U) != 0;
			unsigned char VKeyCode = static_cast<unsigned char>(wparam);
			app->m_input.keyboard.KeyStates[VKeyCode] = true;
			if (!wasDown && app->m_input.keyboard.OnKeyDown.contains(VKeyCode))
			{
				app->m_input.keyboard.OnKeyDown[VKeyCode]();
			}
			return 0;
		}

		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			unsigned char VKeyCode = static_cast<unsigned char>(wparam);
			app->m_input.keyboard.KeyStates[VKeyCode] = false;
			if (app->m_input.keyboard.OnKeyUp.contains(VKeyCode))
			{
				app->m_input.keyboard.OnKeyUp[VKeyCode]();
			}
			return 0;
		}
		case WM_SYSCOMMAND:
			// Disable Alt application menu, if do not this, Alt + other key make a beep sound
			if ((wparam & 0xFFF0) == SC_KEYMENU) { return 0; }
			return DefWindowProc(hwnd, msg, wparam, lparam);

		default: 
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}
}