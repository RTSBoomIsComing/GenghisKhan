#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
class ImGuiManager
{
public:
	static void Initialize(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
	static void Destroy();
	static void Render();
};