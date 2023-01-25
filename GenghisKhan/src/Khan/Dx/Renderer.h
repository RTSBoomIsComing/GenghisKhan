#pragma once
#include <d3d11.h>
namespace Khan
{
	// Renderer is not available now.
	// once multiple windows are needed, Renderer class has to be available.
	struct Renderer
	{
		std::shared_ptr<class Window> m_window;
		
		ComPtr<IDXGISwapChain> m_swapchain;

		// render target
		ComPtr<ID3D11RenderTargetView> m_rtview;

		// depth stencil
		ComPtr<ID3D11DepthStencilState> m_dsstate;
		ComPtr<ID3D11DepthStencilView> m_dsview;

		float aspectRatio{};

	};
}

