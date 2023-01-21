#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <functional>
namespace Khan
{
	class ImGuiManager
	{
	public:
		ImGuiManager(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context)
		{
			// Setup Dear ImGui context
			::IMGUI_CHECKVERSION();
			::ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

			::ImGui::StyleColorsDark(); // Setup Dear ImGui style

			::ImGui_ImplWin32_Init(hwnd);
			::ImGui_ImplDX11_Init(device, device_context);
		}

		~ImGuiManager() noexcept
		{
			::ImGui_ImplDX11_Shutdown();
			::ImGui_ImplWin32_Shutdown();
			::ImGui::DestroyContext();
		}

		void DrawFrame()
		{
			// Start the Dear ImGui frame
			::ImGui_ImplDX11_NewFrame();
			::ImGui_ImplWin32_NewFrame();
			::ImGui::NewFrame();
			this->DrawFrame_Impl(pApp);
			::ImGui::Render();
			::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
		void BindDrawFunc(class Application* pApp, std::function<void(Application*)> fn)
		{
			this->DrawFrame_Impl = fn;
			this->pApp = pApp;
		}

	private:
		std::function<void(Application*)> DrawFrame_Impl;
		Application* pApp;
	};
}
