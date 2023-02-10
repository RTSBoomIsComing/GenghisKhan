#include "pch.h"
#include "Game2.h"

#include <KhanTools/Log.h>

#include <KhanRender/SelectionRectRenderer.h>
#include <KhanRender/CubeRenderer.h>
#include <KhanRender/ImGuiRenderer.h>
#include <KhanRender/MeshRenderer.h>

#include <KhanECS/Camera.h>
#include <KhanECS/Cube.h>
#include <KhanECS/PlayerControl.h>
#include <KhanECS/Systems.h>

// standard libraries
#include <random>

Game2::Game2()
	:
	m_mainRenderer(m_window_handle, m_window_width, m_window_height)
{
	BindActionsToInput();
	m_imGuiRenderer = std::make_unique<KhanRender::ImGuiRenderer>(m_window_handle, m_mainRenderer, std::bind(&Game2::OnImGuiRender, this));
	m_cubeRenderer = std::make_unique<KhanRender::CubeRenderer>(m_mainRenderer);
	m_meshRenderer = std::make_unique<KhanRender::MeshRenderer>(m_mainRenderer);

	auto entity = KhanECS::Entity::MakeCamera(m_reg);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> die(-40, 40);

	for (int i{}; i < 1000; ++i)
	{
		auto e = KhanECS::Entity::MakeCube(m_reg, XMFLOAT3{ (float)die(gen), (float)die(gen), 50.0F + die(gen) });
	}

	//auto e = KhanECS::Entity::MakeCube(m_reg, XMFLOAT3{0.0F, 0.0F, -5.0F});
}

Game2::~Game2() noexcept
{
}

void Game2::Run()
{
	using namespace DirectX;
	using namespace entt::literals;

	auto [LBDown_X, LBDown_Y] = m_input.mouse.GetLastLeftDownPosition();
	selectionRect.left = LBDown_X;
	selectionRect.top = LBDown_Y;

	auto [Move_X, Move_Y] = m_input.mouse.GetLastMovePosition();
	selectionRect.right = Move_X;
	selectionRect.bottom = Move_Y;

	// camera movement control
	m_cameraVelocity = {};
	if (m_isMouseLocked)
	{
		if (Move_X == 0) m_cameraVelocity.x += -1.0F;
		if (Move_Y == 0) m_cameraVelocity.y += +1.0F;
		if (Move_X == m_window_width - 1)  m_cameraVelocity.x += +1.0F;
		if (Move_Y == m_window_height - 1) m_cameraVelocity.y += -1.0F;
	}
	if (m_input.keyboard.KeyStates['A']) m_cameraVelocity.x += -1.0F;
	if (m_input.keyboard.KeyStates['D']) m_cameraVelocity.x += +1.0F;
	if (m_input.keyboard.KeyStates['S']) m_cameraVelocity.y += -1.0F;
	if (m_input.keyboard.KeyStates['W']) m_cameraVelocity.y += +1.0F;

	KhanECS::System::MouseEdgeScroll(m_reg, m_cameraVelocity);
	XMMATRIX viewProjMat = KhanECS::System::GetViewMatrix(m_reg) * KhanECS::System::GetProjectionMatrix(m_aspectRatio);


	std::vector<XMMATRIX> cubeWorldMatrices = std::move(KhanECS::System::GetWorldMatrices<KhanECS::Component::Cube>(m_reg));

	//m_cubeRenderer->Update(cubeWorldMatrices, viewProjMat);
	m_meshRenderer->Update(cubeWorldMatrices, viewProjMat);


	m_mainRenderer.RenderBegin(clear_color);
	//m_cubeRenderer->Render();
	m_meshRenderer->Render();

	static auto selectionRect_renderer = KhanRender::SelectionRectRenderer(m_mainRenderer);
	if (m_isSelectionRectDrawing && m_isMouseLocked)
	{
		selectionRect_renderer.Update(selectionRect, m_window_width, m_window_height);
		selectionRect_renderer.Render();
	}


	m_imGuiRenderer->Render();
	m_mainRenderer.RenderEnd();
}

void Game2::OnResizeWindow(UINT width, UINT height) noexcept
{
	Application::OnResizeWindow(width, height);
	m_mainRenderer.ResizeRenderTarget(width, height);
}

void Game2::OnImGuiRender()
{    // Our state
	// (we use static, which essentially makes the variable globals, as a convenience to keep the example code easy to follow)
	static bool show_demo_window = true;
	static bool show_another_window = false;
	//static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
	// these input bindings are just for testing

	m_input.mouse.OnLeftButtonDown.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("LBD: {:d}, {:d}", x, y));
		m_isSelectionRectDrawing = true;
	};
	m_input.mouse.OnLeftButtonUp.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("LBU: {:d}, {:d}", x, y));
		m_isSelectionRectDrawing = false;
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

}
