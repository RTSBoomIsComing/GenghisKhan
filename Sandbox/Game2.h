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
	class MeshRenderer;
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

protected:
	KhanRender::MainRenderer m_mainRenderer;
	std::unique_ptr <KhanRender::ImGuiRenderer> m_imGuiRenderer;
	std::unique_ptr <KhanRender::CubeRenderer> m_cubeRenderer;
	//std::unique_ptr <KhanRender::SkeletalMeshRenderer> m_ArcherRenderer;
	//std::unique_ptr <KhanRender::SkeletalMeshRenderer> m_KnightRenderer;
	//std::unique_ptr <KhanRender::SkeletalMeshRenderer> m_PaladinRenderer;
	std::unique_ptr<KhanECS::System::SkeletalMeshRenderSystem> m_SkeletalMeshRenderSystem;
protected:
	entt::registry m_reg;

protected:
	DirectX::XMFLOAT3 m_cameraVelocity{};
	DirectX::XMFLOAT3 m_cameraRotation{};
	float clear_color[4]{};
	RECT selectionRect{};
	bool m_isCameraMoving{};
	bool m_isSelectionRectDrawing{};
	POINT m_MouseCursorPos{};
	POINT m_MouseMoveRelative{};
};

