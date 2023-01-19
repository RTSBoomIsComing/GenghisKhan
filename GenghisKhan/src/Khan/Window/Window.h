#pragma once
#include <Windows.h>
#include <string>
namespace Khan {
	
	class Window
	{
	public:
		Window(int width, int height, std::wstring name = L"Genghis Khan");
		~Window() noexcept;

	private:
		std::wstring m_name;
		std::wstring m_class_name = L"MainWindow";

	private:
		int m_width{};
		int m_height{};
		HWND m_hwnd{};
		HINSTANCE m_hinstance{};
	public:
		HWND GetHWnd() const noexcept { return m_hwnd; }
		int GetWidth() const noexcept { return m_width; }
		int GetHeight() const noexcept { return m_height; }
	};
}