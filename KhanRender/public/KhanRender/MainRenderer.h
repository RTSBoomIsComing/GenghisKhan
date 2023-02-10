#pragma once
#include "Renderer.h"
#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace KhanRender
{
	class MainRenderer : public Renderer
	{
	public:
		MainRenderer(HWND hWnd, UINT width, UINT height);

	public:
		void RenderBegin(float clear_color[4]) noexcept;
		void RenderEnd() noexcept;
		void ResizeRenderTarget(UINT width, UINT height) noexcept;

	private:
		ComPtr<IDXGISwapChain> m_pSwapChain;

		ComPtr<ID3D11RenderTargetView> m_pRTV;
		ComPtr<ID3D11DepthStencilView> m_pDSV;

		D3D11_VIEWPORT m_viewport{};


	public:
		ComPtr<ID3D11RenderTargetView>& GetRTV() noexcept { return m_pRTV; }
		ComPtr<ID3D11DepthStencilView>& GetDSV() noexcept { return m_pDSV; }
	};
}

