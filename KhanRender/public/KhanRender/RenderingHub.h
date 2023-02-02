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
		void CreateDeviceAndContext();
		void CreateSwapChain(HWND hwnd, UINT width, UINT height);


	public:
		UINT rt_width{};
		UINT rt_height{};

	public:
		static inline ComPtr<ID3D11Device>			d3d_device;
		static inline ComPtr<ID3D11DeviceContext>	d3d_context;

		ComPtr<ID3D11RenderTargetView>	rtview;
		ComPtr<ID3D11DepthStencilView>	dsview;
		ComPtr<ID3D11DepthStencilState> dsstate;

		D3D11_VIEWPORT viewport;

	private:
		ComPtr<IDXGISwapChain> m_swapchain;
	};
}

