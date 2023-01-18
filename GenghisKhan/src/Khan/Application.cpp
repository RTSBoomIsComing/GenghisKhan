#include "pch.h"
#include "Application.h"

#include "Window/Window.h"
#include "Graphics.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <directxtk/Keyboard.h>
#include <directxtk/Mouse.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace Khan {
	using DirectX::Keyboard;
	using DirectX::Mouse;
	Application::Application(int window_width, int window_height, std::wstring window_name)
	{
		m_window = std::make_unique<Window>(1024, 768, std::move(window_name));

		SetWindowLongPtr(m_window->GetHWnd(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		SetWindowLongPtr(m_window->GetHWnd(), GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc));

		m_keyboard = std::make_unique<Keyboard>();
		m_mouse = std::make_unique<Mouse>();
		m_mouse->SetWindow(m_window->GetHWnd());
		m_graphics = std::make_unique<Graphics>(m_window->GetHWnd(),
			m_window->GetWidth(), m_window->GetHeight());

		InitializeImGui();
	}
	Application::~Application()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::Update()
	{
		auto mouse = m_mouse->GetState();
		if (mouse.leftButton)
		{
			std::cout << mouse.x << ", " << mouse.y << std::endl;
		}
		UpdateImGui();
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
				Update();
				Render();
				Sleep(50);
			}
		}

		return static_cast<int>(msg.wParam);
	}
	void Application::InitializeImGui()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		ImGui::StyleColorsDark(); // Setup Dear ImGui style
		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(m_window->GetHWnd());
		ImGui_ImplDX11_Init(m_graphics->GetDevice().Get(), m_graphics->GetContext().Get());
	}
	void Application::UpdateImGui()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static bool show_demo_window{ true };
		static bool show_another_window{ false };

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
	}
	void Application::Render()
	{
		ImGui::Render();
		m_graphics->RenderStart();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		m_graphics->RenderEnd();
	}
	void Application::OnResizeWindow(UINT width, UINT height)
	{
		m_graphics->ResizeBackBuffers(width, height);
	}

	LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
			if (wparam != SIZE_MINIMIZED)
			{
				auto app = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				app->OnResizeWindow((UINT)LOWORD(lparam), (UINT)HIWORD(lparam));
			}
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATEAPP:
			Keyboard::ProcessMessage(msg, wparam, lparam);
			Mouse::ProcessMessage(msg, wparam, lparam);
			break;
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
			Mouse::ProcessMessage(msg, wparam, lparam);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(msg, wparam, lparam);
			break;
		case WM_SYSKEYDOWN:
			Keyboard::ProcessMessage(msg, wparam, lparam);
			if (wparam == VK_RETURN && (lparam & 0x60000000) == 0x20000000)
			{
			}
			break;

		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0u;
	}
}
