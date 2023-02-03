#include "pch.h"
#include "Game2.h"
#include <KhanTools/Log.h>
#include <KhanRender/RenderingHub.h>
#include <KhanRender/SelectionRectRenderer.h>
#include <KhanRender/CubeRenderer.h>

Game2::Game2()
	:
	m_renderingHub(std::make_shared<KhanRender::RenderingHub>(m_window_handle, m_window_width, m_window_height))
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
		x2 = x;
		y2 = y;
		if (x == 0 || x == (m_window_width - 1) || y == 0 || y == (m_window_height - 1))
		{
			bIsMouseOnEdge = true;
		}
		else
		{
			bIsMouseOnEdge = false;
		}
	};
}

Game2::~Game2() noexcept
{
}

void Game2::Run()
{
	m_renderingHub->RenderBegin();

	static auto cube_renderer = KhanRender::CubeRenderer(m_renderingHub);
	cube_renderer.Render();

	static auto selectionRect_renderer = KhanRender::SelectionRectRenderer(m_renderingHub);
	if (bIsSelectionRectDrawing)
		selectionRect_renderer.Render(x1, y1, x2, y2);
	
	m_renderingHub->RenderEnd();
}

void Game2::OnResizeWindow(UINT width, UINT height) noexcept
{
	Application::OnResizeWindow(width, height);
	m_renderingHub->ResizeRenderTarget(width, height);
}
