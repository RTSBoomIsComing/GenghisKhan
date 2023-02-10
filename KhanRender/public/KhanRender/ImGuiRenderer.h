#pragma once
#include "Renderer.h"

#include <functional>
#include <memory>
#include <imgui.h>

namespace KhanRender
{
	class ImGuiRenderer
	{
	public:
		ImGuiRenderer(void* hWnd, const Renderer& renderer, std::function<void()> ImGuiRender_Impl);
		~ImGuiRenderer() noexcept;

		void Render();

	private:
		std::function<void()> m_ImGuiRender_Impl;
	};
}