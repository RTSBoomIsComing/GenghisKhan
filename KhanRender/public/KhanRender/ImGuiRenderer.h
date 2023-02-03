#pragma once
#include <functional>
#include <memory>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "KhanRender/RenderingHub.h"

namespace KhanRender
{
	class ImGuiRenderer
	{
	public:
		ImGuiRenderer(void* hwnd, std::shared_ptr<RenderingHub> core, std::function<void()> ImGuiRender_Impl)
			:
			m_ImGuiRender_Impl(ImGuiRender_Impl)
		{
			// Setup Dear ImGui context
			::IMGUI_CHECKVERSION();
			::ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

			::ImGui::StyleColorsDark(); // Setup Dear ImGui style

			::ImGui_ImplWin32_Init(hwnd);
			::ImGui_ImplDX11_Init(core->GetDevice(), core->GetContext());
		}

		~ImGuiRenderer() noexcept
		{
			::ImGui_ImplDX11_Shutdown();
			::ImGui_ImplWin32_Shutdown();
			::ImGui::DestroyContext();
		}

		void Render()
		{
			::ImGui_ImplDX11_NewFrame();
			::ImGui_ImplWin32_NewFrame();
			::ImGui::NewFrame();
			m_ImGuiRender_Impl();
			::ImGui::Render();
			::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

	private:
		std::function<void()> m_ImGuiRender_Impl;
	};
}