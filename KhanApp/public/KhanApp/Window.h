#pragma once
#include <Windows.h>
#include <string>
namespace KhanApp
{
	class Window
	{
	public:
		Window(int width = 1024, int height = 768, std::wstring name = L"window");
		virtual ~Window() noexcept;
		void EnableMouseLockToWindow();
		void EnableMouseLockAtPoint(int x, int y);
		void DisableMouseLock();
	private:
		std::wstring m_name;
		std::wstring m_class_name = L"MainWindow";

	protected:
		bool m_isMouseLocked{};
		HWND m_window_handle{};
		int m_window_width{};
		int m_window_height{};
	};
}