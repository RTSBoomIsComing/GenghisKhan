#include "pch.h"
#include "Window.h"
#include "DxUtility.h"

namespace Khan {
	Window::Window(int width, int height, std::wstring name)
		:
		m_width(width), m_height(height), m_name(std::move(name)),
		m_hinstance(GetModuleHandle(nullptr))
	{
		WNDCLASSEXW wc{};
		wc.cbClsExtra = 0;
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.cbWndExtra = 0;
		wc.lpszClassName = m_class_name.data();
		wc.lpfnWndProc = DefWindowProc;
		if (RegisterClassExW(&wc) == 0)
		{
			MessageBoxW(nullptr, L"failed to register window class", L"Failed", 0u);
			throw GetLastError();
		}

		RECT rect{ 0, 0, m_width, m_height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		m_hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, m_class_name.data(), m_name.data(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top,
			nullptr, nullptr, m_hinstance, nullptr);

		if (!m_hwnd)
		{
			KHAN_ERROR("failed to create window");
			throw GetLastError();
		}

		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);
	}

	Window::~Window() noexcept
	{
		DestroyWindow(m_hwnd);
		UnregisterClass(m_class_name.data(), m_hinstance);
	}
}