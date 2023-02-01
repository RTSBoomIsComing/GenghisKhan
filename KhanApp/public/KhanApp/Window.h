#pragma once
#include <Windows.h>
#include <string>
namespace KhanApp
{
	class Window
	{
	public:
		Window(int width = 1024, int height = 768, std::wstring name = L"window");
		~Window() noexcept;
		void EnableMouseLockToWindow();
		void DisableMouseLockToWindow();
	private:
		std::wstring m_name;
		std::wstring m_class_name = L"MainWindow";

	protected:
		bool bIsMouseLocked{};
		HWND m_window_handle{};
		int m_window_width{};
		int m_window_height{};
	};
}