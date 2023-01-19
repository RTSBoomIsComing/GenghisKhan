#pragma once
#include <Windows.h>
#include <string>
namespace Khan {
	
	class Window
	{
	public:
		Window(int width = 1024, int height = 768, std::wstring name = L"window");
		~Window() noexcept;

	private:
		std::wstring m_name;
		std::wstring m_class_name = L"MainWindow";

	private:
		HWND m_hwnd{};
		int m_width{};
		int m_height{};

	public:
		HWND GetHWnd() const noexcept { return m_hwnd; }
		int GetWidth() const noexcept { return m_width; }
		int GetHeight() const noexcept { return m_height; }
		void SetWidth(int width) noexcept { m_width = width; }
		void SetHeight(int height) noexcept { m_height = height; }

	};
}