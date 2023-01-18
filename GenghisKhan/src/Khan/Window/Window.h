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
		inline HWND GetHWnd() const { return m_hwnd; }
		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
	};
}