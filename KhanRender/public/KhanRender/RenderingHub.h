#pragma once

#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace KhanRender
{
	class RenderingHub
	{
	public:
		RenderingHub(HWND hwnd, UINT width, UINT height);

	public:
		void RenderBegin() noexcept;
		void RenderEnd() noexcept;
		void ResizeRenderTarget(UINT width, UINT height) noexcept;

	private:
		UINT screen_width{};
		UINT screen_height{};

		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;
		ComPtr<IDXGISwapChain> m_swapchain;

		ComPtr<ID3D11RenderTargetView> m_rtview;
		ComPtr<ID3D11DepthStencilView> m_dsview;

		D3D11_VIEWPORT m_viewport;


	public:
		UINT GetScreenWidth() const noexcept { return screen_width; }
		UINT GetScreenHeight() const noexcept { return screen_height; }

		ComPtr<ID3D11Device> GetDevice() noexcept { return m_device; }
		ComPtr<ID3D11DeviceContext> GetContext() noexcept { return m_context; }
		ComPtr<ID3D11RenderTargetView> GetRTView() noexcept { return m_rtview; }
		ComPtr<ID3D11DepthStencilView> GetDSView() noexcept { return m_dsview; }
	};
}

