#pragma once
#include <KhanApp/Application.h>

// standard libraries
#include <memory>

// additional dependencies
#include <entt/fwd.hpp>
#include <DirectXMath.h>

namespace KhanRender
{
	class RenderingHub;
	class ImGuiRenderer;
	class CubeRenderer;
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
	std::shared_ptr<KhanRender::RenderingHub> m_renderingHub;
	std::unique_ptr<KhanRender::ImGuiRenderer> m_imGuiRenderer;
	std::unique_ptr <KhanRender::CubeRenderer> m_cubeRenderer;
protected:
	entt::registry m_reg;

protected:
	DirectX::XMFLOAT2 m_cameraVelocity{};

	int x1{};
	int x2{};
	int y1{};
	int y2{};
	bool m_isCameraMoving{};
	bool m_isSelectionRectDrawing{};
};

