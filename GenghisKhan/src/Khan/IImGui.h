#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace Khan
{
	class IImGui abstract
	{
	public:
		IImGui()
		{
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

			ImGui::StyleColorsDark(); // Setup Dear ImGui style
		}

		virtual ~IImGui() noexcept
		{
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}

		void Draw()
		{
			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			Draw_Impl();
			ImGui::Render();
		}

		virtual void Draw_Impl() abstract;
	};
}
