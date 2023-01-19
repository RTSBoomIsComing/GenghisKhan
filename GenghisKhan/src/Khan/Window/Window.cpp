#include "pch.h"
#include "Window.h"
#include "Common/DxUtility.h"

using namespace Khan;

Window::Window(int width, int height, std::wstring name)
	:
	m_width(width), m_height(height), m_name(std::move(name))
{
	WNDCLASSEXW wc{};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = m_class_name.data();
	if (!RegisterClassExW(&wc))
	{
		KHAN_ERROR("failed to register window class");
		throw GetLastError();
	}
	
	RECT rect{ 0, 0, width, height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	int adjusted_width{ rect.right - rect.left };
	int adjusted_height{ rect.bottom - rect.top };
	m_hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW, m_class_name.data(), m_name.data(),
		WS_OVERLAPPEDWINDOW, 300, 20,
		adjusted_width, adjusted_height,
		nullptr, nullptr, wc.hInstance, nullptr);

	if (!m_hwnd)
	{
		KHAN_ERROR("failed to create window");
		throw GetLastError();
	}

	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);
}

Window::~Window() noexcept
{
	DestroyWindow(m_hwnd);
	UnregisterClass(m_class_name.data(), nullptr);
}
