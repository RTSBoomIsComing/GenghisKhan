#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <functional>


template<typename T> concept have_ImGuiRender_Impl = requires(T m)
{
	m.ImGuiRender_Impl();
};

template<typename T> requires have_ImGuiRender_Impl<T>
class ImGuiManager
{
public:
	ImGuiManager(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context, T* pOwner)
	{
		// Setup Dear ImGui context
		::IMGUI_CHECKVERSION();
		::ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

		::ImGui::StyleColorsDark(); // Setup Dear ImGui style

		::ImGui_ImplWin32_Init(hwnd);
		::ImGui_ImplDX11_Init(device, device_context);

		this->pOwner = pOwner;
	}

	~ImGuiManager() noexcept
	{
		::ImGui_ImplDX11_Shutdown();
		::ImGui_ImplWin32_Shutdown();
		::ImGui::DestroyContext();
	}

	void Render()
	{
		// Start the Dear ImGui frame
		::ImGui_ImplDX11_NewFrame();
		::ImGui_ImplWin32_NewFrame();
		::ImGui::NewFrame();
		//this->DrawFrame_Impl(pApp);
		pOwner->ImGuiRender_Impl();
		::ImGui::Render();
		::ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	/*void BindDrawFunc(T* pApp, std::function<void(T*)> fn)
	{
		this->DrawFrame_Impl = fn;
		this->pApp = pApp;
	}*/

private:
	//std::function<void(T*)> DrawFrame_Impl;
	T* pOwner{};
};
