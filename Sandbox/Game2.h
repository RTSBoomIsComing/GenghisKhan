#pragma once
#include <KhanApp/Application.h>

#include <memory>

namespace KhanRender
{
	class RenderingHub;
}

class Game2 : public KhanApp::Application
{
public:
	Game2();
	~Game2() noexcept;

public:
	void Run();
	void OnResizeWindow(UINT width,UINT height) noexcept override;

	int x1{};
	int x2{};
	int y1{};
	int y2{};
	bool bIsSelectionRectDrawing{};
protected:
	std::shared_ptr<KhanRender::RenderingHub> m_renderingHub;

protected:
	bool bIsMouseOnEdge{};
};

