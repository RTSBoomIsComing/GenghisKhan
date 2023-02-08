#include "pch.h"
#include "Game2.h"
#include <KhanTools/Log.h>
#include <KhanRender/RenderingHub.h>
#include <KhanRender/SelectionRectRenderer.h>
#include <KhanRender/CubeRenderer.h>
#include <KhanRender/ImGuiRenderer.h>

#include <KhanECS/Camera.h>
#include <KhanECS/PlayerControl.h>

Game2::Game2()
	:
	m_renderingHub(std::make_shared<KhanRender::RenderingHub>(m_window_handle, m_window_width, m_window_height))
{
	BindActionsToInput();
	m_imGuiRenderer = std::make_unique<KhanRender::ImGuiRenderer>(m_window_handle, m_renderingHub, std::bind(&Game2::OnImGuiRender, this));


	auto entity = KhanECS::Entity::MakeCamera(m_reg);
}

Game2::~Game2() noexcept
{
}

void Game2::Run()
{
	using namespace DirectX;

	KhanECS::System::MouseEdgeScroll(m_reg, m_cameraVelocity);

	XMMATRIX viewMat = KhanECS::System::GetViewMatrix(m_reg);
	XMMATRIX viewProjMat = viewMat * KhanECS::System::GetProjectionMatrix(m_aspectRatio);

	static auto cube_renderer = KhanRender::CubeRenderer(m_renderingHub);
	{
		static float angle_temp{ -5.0F };
		angle_temp += 0.02F;
		if (angle_temp > 5.0F) angle_temp = -5.0F;
		std::vector<XMFLOAT4X4> cubeTransforms(8);
		int idx{};
		XMStoreFloat4x4(&cubeTransforms[idx++], XMMatrixRotationY(+angle_temp) * XMMatrixTranslation(+angle_temp, 0.0F, 0.0F));
		XMStoreFloat4x4(&cubeTransforms[idx++], XMMatrixRotationY(-angle_temp) * XMMatrixTranslation(-angle_temp, 0.0F, 0.0F));
		XMStoreFloat4x4(&cubeTransforms[idx++], XMMatrixRotationY(-angle_temp) * XMMatrixTranslation(-angle_temp, 2.0F, 0.0F));
		XMStoreFloat4x4(&cubeTransforms[idx++], XMMatrixRotationY(+angle_temp) * XMMatrixTranslation(+angle_temp, 2.0F, 0.0F));
		XMStoreFloat4x4(&cubeTransforms[idx++], XMMatrixRotationY(+angle_temp) * XMMatrixTranslation(+angle_temp, -2.0F, 0.0F));
		XMStoreFloat4x4(&cubeTransforms[idx++], XMMatrixRotationY(-angle_temp) * XMMatrixTranslation(-angle_temp, -2.0F, 0.0F));
		XMStoreFloat4x4(&cubeTransforms[idx++], XMMatrixRotationY(-angle_temp) * XMMatrixTranslation(-angle_temp + 3.0F, 2.0F, 0.0F));
		XMStoreFloat4x4(&cubeTransforms[idx++], XMMatrixRotationY(+angle_temp) * XMMatrixTranslation(+angle_temp + 3.0F, 2.0F, 0.0F));
		cube_renderer.Update(cubeTransforms, viewProjMat);
	}
	m_renderingHub->RenderBegin();
	cube_renderer.Render();

	static auto selectionRect_renderer = KhanRender::SelectionRectRenderer(m_renderingHub);
	if (bIsSelectionRectDrawing)
	{
		selectionRect_renderer.Update(x1, y1, x2, y2);
		selectionRect_renderer.Render();
	}

	
	m_imGuiRenderer->Render();
	m_renderingHub->RenderEnd();
}

void Game2::OnResizeWindow(UINT width, UINT height) noexcept
{
	Application::OnResizeWindow(width, height);
	m_renderingHub->ResizeRenderTarget(width, height);
}

void Game2::OnImGuiRender()
{    // Our state
	// (we use static, which essentially makes the variable globals, as a convenience to keep the example code easy to follow)
	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

}

void Game2::BindActionsToInput() noexcept
{
	m_input.mouse.OnLeftButtonDown.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("LBD: {:d}, {:d}", x, y));
		x1 = x;
		y1 = y;
		
		bIsSelectionRectDrawing = true;
	};
	m_input.mouse.OnLeftButtonUp.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("LBU: {:d}, {:d}", x, y));
		bIsSelectionRectDrawing = false;
	};
	m_input.mouse.OnRightButtonDown.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("RBD: {:d}, {:d}", x, y));
	};
	m_input.mouse.OnRightButtonUp.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("RBU: {:d}, {:d}", x, y));
	};
	m_input.mouse.OnLeftButtonDown.InstantFn = [&](int x, int y) {
		KHAN_ERROR(std::format("LBD: {:d}, {:d}", x, y));
		m_input.mouse.OnLeftButtonDown.InstantFn = nullptr;
	};
	m_input.mouse.OnRightButtonDown.InstantFn = [&](int x, int y) {
		KHAN_ERROR(std::format("RBD: {:d}, {:d}", x, y));
		m_input.mouse.OnRightButtonDown.InstantFn = nullptr;
	};
	m_input.mouse.OnLeftButtonUp.InstantFn = [&](int x, int y) {
		KHAN_ERROR(std::format("LBU: {:d}, {:d}", x, y));
		m_input.mouse.OnLeftButtonUp.InstantFn = nullptr;
	};
	m_input.mouse.OnRightButtonUp.InstantFn = [&](int x, int y) {
		KHAN_ERROR(std::format("RBU: {:d}, {:d}", x, y));
		m_input.mouse.OnRightButtonUp.InstantFn = nullptr;
	};
	m_input.mouse.OnMouseMove.DefaultFn = [&](int x, int y) {
		using namespace DirectX;

		if (!m_isMouseLocked) return;

		x2 = x; y2 = y;

		m_cameraVelocity = DirectX::XMFLOAT2{};
		if (x == 0) m_cameraVelocity.x = -1.0F;
		if (y == 0) m_cameraVelocity.y = +1.0F;
		if (x == m_window_width - 1)  m_cameraVelocity.x = +1.0F;
		if (y == m_window_height - 1) m_cameraVelocity.y = -1.0F;
	};
}
