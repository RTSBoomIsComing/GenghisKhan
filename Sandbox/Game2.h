#pragma once
#include <KhanApp/Application.h>
#include <KhanRender/MainRenderer.h>
// standard libraries
#include <memory>

// additional dependencies
#include <entt/fwd.hpp>
#include <DirectXMath.h>

namespace KhanRender
{
	class ImGuiRenderer;
	class CubeRenderer;
	class GridFloorRenderer;
	class SkeletalMeshRenderer;
}

namespace KhanECS::System
{
	class SkeletalMeshRenderSystem;
}

class Game2 : public KhanApp::Application
{
public:
	Game2();
	virtual ~Game2() noexcept;

public:
	void Run();
	virtual void OnResizeWindow(UINT width, UINT height) noexcept override;
	void OnImGuiRender();

protected:
	void BindActionsToInput() noexcept;
	void ProcessInput() noexcept;

protected:
	KhanRender::MainRenderer m_mainRenderer;
	std::unique_ptr <KhanRender::ImGuiRenderer> m_imGuiRenderer;
	std::unique_ptr <KhanRender::CubeRenderer> m_cubeRenderer;
	std::unique_ptr <KhanRender::GridFloorRenderer> m_GridFloorRenderer;

	std::shared_ptr<KhanRender::SkeletalMeshRenderer> m_SkeletalMeshRenderer_Archer;
	std::unique_ptr<KhanECS::System::SkeletalMeshRenderSystem> m_SkeletalMeshRenderSystem;
protected:
	entt::registry m_reg;

protected:
	struct
	{
		DirectX::XMFLOAT3 CameraVelocity{};
		DirectX::XMFLOAT3 CameraRotation{};
		float clear_color[4]{};
		RECT SelectionRect{};
		bool bIsCameraMoving{};
		bool bIsSelectionRectDrawing{};

		POINT LastMousePos{};
		POINT MouseMoveRelative{};
	} m_GameInfo;

};

