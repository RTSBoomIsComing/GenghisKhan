#include "pch.h"
#include "Game2.h"

#include "KhanECS/SkeletalMeshRenderSystem.h"

#include <KhanTools/Log.h>

#include <KhanRender/NearPlaneRenderer.h>

#include <KhanRender/SelectionRectRenderer.h>
#include <KhanRender/CubeRenderer.h>
#include <KhanRender/ImGuiRenderer.h>
#include <KhanRender/GridFloorRenderer.h>
#include <KhanRender/MeshRenderer.h>
#include <KhanRender/SkeletalMeshRenderer.h>
#include <KhanRender/LaserRenderer.h>

#include <KhanECS/Camera.h>
#include <KhanECS/Cube.h>
#include <KhanECS/PlayerControl.h>
#include <KhanECS/Character.h>

// standard libraries
#include <random>
#include <chrono>

float DEBUGSCALAR{};
Game2::Game2()
	:
	m_mainRenderer(m_window_handle, m_window_width, m_window_height)
{
	BindActionsToInput();

	m_SkeletalMeshRenderer_Archer = std::make_shared<KhanRender::SkeletalMeshRenderer>(m_mainRenderer, "D:\\Assets\\Mixamo\\Archer\\Erika Archer With Bow Arrow.fbx");
	m_LaserRenderer = std::make_unique<KhanRender::LaserRenderer>(m_mainRenderer);
	m_imGuiRenderer = std::make_unique<KhanRender::ImGuiRenderer>(m_window_handle, m_mainRenderer, std::bind(&Game2::OnImGuiRender, this));

	KhanRender::NearPlaneRenderer nearPlaneRenderer(m_mainRenderer);
	m_GridFloorRenderer = std::make_unique<KhanRender::GridFloorRenderer>(nearPlaneRenderer);
	//m_cubeRenderer = std::make_unique<KhanRender::CubeRenderer>(m_mainRenderer);


	m_SkeletalMeshRenderSystem = std::make_unique<KhanECS::System::SkeletalMeshRenderSystem>();

	auto entity = KhanECS::Entity::MakeCamera(m_reg);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> die(-1500.0F, 1500.0F);
	for (int i{}; i < 100; ++i)
	{
		auto e = KhanECS::Entity::MakeCharacter(m_reg, XMFLOAT3{ die(gen), 0.0F, 500.0F + die(gen) });
		m_reg.emplace<KhanECS::Component::Archer>(e);
		m_reg.emplace<KhanECS::Component::SkeletalMeshComponent>(e, m_SkeletalMeshRenderer_Archer);
	}

	// for test
	m_LaserRenderer->GetVertices().emplace_back(XMFLOAT3{ 300.0F, 300.0F, 300.0F }, XMFLOAT3{ 0.0F, 0.0F, 0.0F });
	m_LaserRenderer->GetVertices().emplace_back(XMFLOAT3{ 500.0F, 300.0F, 300.0F }, XMFLOAT3{ 0.0F, 0.0F, 0.0F });
}

Game2::~Game2() noexcept
{
}

void Game2::Run()
{
	using namespace DirectX;

	static auto prevTimePoint = std::chrono::steady_clock::now();
	const auto currTimePoint = std::chrono::steady_clock::now();
	const std::chrono::duration<float> deltaTime = currTimePoint - prevTimePoint;
	prevTimePoint = currTimePoint;

	ProcessInput();
	KhanECS::System::SetCameraRotation(m_reg, m_GameInfo.CameraRotation);
	KhanECS::System::MouseEdgeScroll(m_reg, m_GameInfo.CameraVelocity);

	m_GridFloorRenderer->Update(XMMatrixInverse(nullptr, KhanECS::System::GetViewProjectionMatrix(m_reg)));
	m_SkeletalMeshRenderSystem->Update(deltaTime.count(), m_reg);

	// update renderer, upload data from cpu(system memory) to gpu(video memory)
	m_SkeletalMeshRenderer_Archer->Update(KhanECS::System::GetViewProjectionMatrix(m_reg));
	m_LaserRenderer->Update(KhanECS::System::GetViewProjectionMatrix(m_reg));

	// start rendering
	m_mainRenderer.RenderBegin(m_GameInfo.clear_color);
	m_SkeletalMeshRenderer_Archer->Render();
	m_GridFloorRenderer->Render();
	m_LaserRenderer->Render();

	static auto selectionRect_renderer = KhanRender::SelectionRectRenderer(m_mainRenderer);
	if (m_PlayerController.LButtonDown && m_isMouseLocked)
	{
		selectionRect_renderer.Update(m_GameInfo.SelectionRect, m_window_width, m_window_height);
		selectionRect_renderer.Render();
	}

	m_imGuiRenderer->Render();
	m_mainRenderer.RenderEnd();

	m_GameInfo.CameraVelocity = {};

}

void Game2::OnResizeWindow(UINT width, UINT height) noexcept
{
	Application::OnResizeWindow(width, height);
	m_mainRenderer.ResizeRenderTarget(width, height);
	auto view = m_reg.view<KhanECS::Component::CameraProjectionInfo>();
	for (auto& e : view)
	{
		auto& projInfo = view.get<KhanECS::Component::CameraProjectionInfo>(e);
		projInfo.aspectRatio = m_aspectRatio;
		projInfo.ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(projInfo.fovAngleY, projInfo.aspectRatio, projInfo.nearZ, projInfo.farZ);
	}
}

void Game2::OnImGuiRender()
{
	using namespace DirectX;
	// Our state
	// (we use static, which essentially makes the variable globals, as a convenience to keep the example code easy to follow)
	static bool show_demo_window = true;
	static bool show_another_window = false;


	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("DebugScalar", &DEBUGSCALAR, 0.0F, 500.F);
		ImGui::SliderFloat3("camera rot", reinterpret_cast<float*>(&m_GameInfo.CameraRotation), -XM_PI, XM_PI);
		ImGui::ColorEdit3("clear color", (float*)&m_GameInfo.clear_color); // Edit 3 floats representing a color

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
	using namespace DirectX;

	m_input.mouse.OnLeftButtonDown.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("LBD: {:d}, {:d}", x, y));
		if (!m_PlayerController.LButtonDown)
		{
			m_PlayerController.LButtonDown = { x, y };
		}
	};
	m_input.mouse.OnLeftButtonUp.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("LBU: {:d}, {:d}", x, y));
		auto& LButtonDown = m_PlayerController.LButtonDown;
		if (!LButtonDown) { return; }
		if (LButtonDown->x == x && LButtonDown->y == y)
		{
			m_PlayerController.ClickingPoints.emplace_back(x, y);
		}
		else
		{
			m_PlayerController.DraggingRects.emplace_back(LButtonDown->x, LButtonDown->y, x, y);
		}
		LButtonDown.reset();
	};
	m_input.mouse.OnRightButtonDown.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("RBD: {:d}, {:d}", x, y));
	};
	m_input.mouse.OnRightButtonUp.DefaultFn = [&](int x, int y) {
		KHAN_INFO(std::format("RBU: {:d}, {:d}", x, y));
	};

	m_input.mouse.OnMouseMove.DefaultFn = [&](int x, int y) {
		if (m_input.keyboard.KeyStates[VK_MENU/*ALT key*/] != true)
		{
			m_PlayerController.MouseMove = { x, y };
		}
	};

	m_input.mouse.OnMouseRawInput.DefaultFn = [&](int x, int y) {
		if (m_input.keyboard.KeyStates[VK_MENU/*ALT key*/])
		{
			m_PlayerController.MouseMoveRelative.x = +x;
			m_PlayerController.MouseMoveRelative.y = +y;
		}
	};

	m_input.mouse.OnMouseWheel.DefaultFn = [&](int x, int y, int delta) {
		m_GameInfo.CameraVelocity.z += static_cast<float>(delta) / 120;
	};
	//m_input.mouse.OnLeftButtonDown.InstantFn = [&](int x, int y) {
	//	KHAN_ERROR(std::format("LBD: {:d}, {:d}", x, y));
	//	m_input.mouse.OnLeftButtonDown.InstantFn = nullptr;
	//};
	//m_input.mouse.OnRightButtonDown.InstantFn = [&](int x, int y) {
	//	KHAN_ERROR(std::format("RBD: {:d}, {:d}", x, y));
	//	m_input.mouse.OnRightButtonDown.InstantFn = nullptr;
	//};
	//m_input.mouse.OnLeftButtonUp.InstantFn = [&](int x, int y) {
	//	KHAN_ERROR(std::format("LBU: {:d}, {:d}", x, y));
	//	m_input.mouse.OnLeftButtonUp.InstantFn = nullptr;
	//};
	//m_input.mouse.OnRightButtonUp.InstantFn = [&](int x, int y) {
	//	KHAN_ERROR(std::format("RBU: {:d}, {:d}", x, y));
	//	m_input.mouse.OnRightButtonUp.InstantFn = nullptr;
	//};

	m_input.keyboard.OnKeyDown[VK_MENU].DefaultFn = [&]() {
		if (m_isMouseLocked)
		{
			while (::ShowCursor(false) >= 0);
			EnableMouseLockAtPoint(m_PlayerController.MouseMove.x, m_PlayerController.MouseMove.y);
		}
	};
	m_input.keyboard.OnKeyUp[VK_MENU].DefaultFn = [&]() {
		if (m_isMouseLocked)
		{
			while (::ShowCursor(true) < 0);
			DisableMouseLock();
			EnableMouseLockToWindow();
		}
	};
}

void Game2::ProcessInput() noexcept
{
	using namespace DirectX;

	if (m_PlayerController.LButtonDown)
	{
		m_GameInfo.SelectionRect.x = m_PlayerController.LButtonDown->x;
		m_GameInfo.SelectionRect.y = m_PlayerController.LButtonDown->y;
		m_GameInfo.SelectionRect.z = m_PlayerController.MouseMove.x;
		m_GameInfo.SelectionRect.w = m_PlayerController.MouseMove.y;
	}

	// camera control by mouse
	m_GameInfo.CameraRotation.x += m_PlayerController.MouseMoveRelative.y * 0.004F;
	m_GameInfo.CameraRotation.y += m_PlayerController.MouseMoveRelative.x * 0.002F;

	if (m_input.keyboard.KeyStates[VK_MENU/*ALT key*/] != true && m_isMouseLocked)
	{
		if (m_PlayerController.MouseMove.x == 0) { m_GameInfo.CameraVelocity.x += -1.0F; }
		if (m_PlayerController.MouseMove.y == 0) { m_GameInfo.CameraVelocity.y += +1.0F; }
		if (m_PlayerController.MouseMove.x == m_window_width - 1) { m_GameInfo.CameraVelocity.x += +1.0F; }
		if (m_PlayerController.MouseMove.y == m_window_height - 1) { m_GameInfo.CameraVelocity.y += -1.0F; }
	}

	// camera control by keyboard
	if (m_input.keyboard.KeyStates['A']) { m_GameInfo.CameraVelocity.x += -1.0F; }
	if (m_input.keyboard.KeyStates['D']) { m_GameInfo.CameraVelocity.x += +1.0F; }
	if (m_input.keyboard.KeyStates['S']) { m_GameInfo.CameraVelocity.y += -1.0F; }
	if (m_input.keyboard.KeyStates['W']) { m_GameInfo.CameraVelocity.y += +1.0F; }

	// reset every frame
	m_PlayerController.MouseMoveRelative = {};
}
