#pragma once
#include <KhanApp/Application.h>
#include <KhanRender/MainRenderer.h>
// standard libraries
#include <memory>
#include <optional>

// additional dependencies
#include <entt/fwd.hpp>
#include <DirectXMath.h>

namespace KhanRender
{
	class ImGuiRenderer;
	class CubeRenderer;
	class GridFloorRenderer;
	class SkeletalMeshRenderer;
	class LaserRenderer;
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
	std::unique_ptr<KhanRender::ImGuiRenderer> m_imGuiRenderer;
	std::unique_ptr<KhanRender::CubeRenderer> m_cubeRenderer;
	std::unique_ptr<KhanRender::GridFloorRenderer> m_GridFloorRenderer;
	std::unique_ptr<KhanRender::LaserRenderer> m_LaserRenderer;
	std::shared_ptr<KhanRender::SkeletalMeshRenderer> m_SkeletalMeshRenderer_Archer;

	std::unique_ptr<KhanECS::System::SkeletalMeshRenderSystem> m_SkeletalMeshRenderSystem; // will be deprecated
protected:
	entt::registry m_reg;

protected:
	struct
	{
		DirectX::XMFLOAT3 CameraVelocity{};
		DirectX::XMFLOAT3 CameraRotation{};
		float clear_color[4]{};
		DirectX::XMINT4 SelectionRect{};
		bool bIsCameraMoving{};
	} m_GameInfo;

	struct
	{
		DirectX::XMINT2 MouseMove{};
		DirectX::XMINT2 MouseMoveRelative{};
		std::optional<DirectX::XMINT2> LButtonDown{};
		std::optional<DirectX::XMINT2> RButtonDown{};
		std::vector<DirectX::XMINT4> DraggingRects;
		std::vector<DirectX::XMINT2> ClickingPoints; // need to reset every frame
	} m_PlayerController;
};

