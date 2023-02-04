#pragma once

#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace KhanRender
{
	class RenderingHub
	{
	public:
		RenderingHub(HWND hWnd, UINT width, UINT height);

	public:
		void RenderBegin() noexcept;
		void RenderEnd() noexcept;
		void ResizeRenderTarget(UINT width, UINT height) noexcept;

	private:
		UINT m_screenWidth{};
		UINT m_screenHeight{};

		ComPtr<ID3D11Device> m_pDevice;
		ComPtr<ID3D11DeviceContext> m_pDeviceContext;
		ComPtr<IDXGISwapChain> m_pSwapChain;

		ComPtr<ID3D11RenderTargetView> m_pRTV;
		ComPtr<ID3D11DepthStencilView> m_pDSV;

		D3D11_VIEWPORT m_viewport{};


	public:
		UINT const& GetScreenWidth() const noexcept { return m_screenWidth; }
		UINT const& GetScreenHeight() const noexcept { return m_screenHeight; }

		ComPtr<ID3D11Device>& GetDevice() noexcept { return m_pDevice; }
		ComPtr<ID3D11DeviceContext>& GetDeviceContext() noexcept { return m_pDeviceContext; }
		ComPtr<ID3D11RenderTargetView>& GetRTV() noexcept { return m_pRTV; }
		ComPtr<ID3D11DepthStencilView>& GetDSV() noexcept { return m_pDSV; }
	};
}

