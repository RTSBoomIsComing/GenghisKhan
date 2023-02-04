#pragma once
#include <functional>
#include <memory>
#include <imgui.h>

namespace KhanRender
{
	class ImGuiRenderer
	{
	public:
		ImGuiRenderer(void* hwnd, std::shared_ptr<class RenderingHub> core, std::function<void()> ImGuiRender_Impl);
		~ImGuiRenderer() noexcept;

		void Render();

	private:
		std::function<void()> m_ImGuiRender_Impl;
	};
}