#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <windef.h>
#include <memory>
#include <cassert>

using Microsoft::WRL::ComPtr;

namespace Khan
{
	class DxCore
	{
	public:
		DxCore(HWND hwnd, int width, int height) noexcept;
		void SwapBuffers() noexcept;
		void ResizeBackBuffers(UINT width, UINT height) noexcept;

	private:
		void InitializeDepthStencil(UINT width, UINT height) noexcept;
		void InitializeRenderTarget() noexcept;
		void InitializeViewport(float width, float height) noexcept;
		void InitializeRSState() noexcept;
	private:
		ComPtr<ID3D11Device>			m_device;
		ComPtr<IDXGISwapChain>			m_swapchain;
		ComPtr<ID3D11DeviceContext>		m_context;
		ComPtr<ID3D11RenderTargetView>	m_rtview;
		ComPtr<ID3D11DepthStencilView>	m_dsview;
		ComPtr<ID3D11DepthStencilState> m_dsstate;
		ComPtr<ID3D11RasterizerState>	m_rsstate;

	public:
		ComPtr<ID3D11Device>& GetDevice() noexcept { return m_device; }
		ComPtr<ID3D11DeviceContext>& GetContext() noexcept { return m_context; }
		ComPtr<ID3D11RenderTargetView>& GetRTView() noexcept { return m_rtview; }
		ComPtr<ID3D11DepthStencilView>& GetDSView() noexcept { return m_dsview; }
		ComPtr<ID3D11DepthStencilState>& GetDSState() noexcept { return m_dsstate; }
		ComPtr<ID3D11RasterizerState>& GetRSState() noexcept { return m_rsstate; }
	};

	extern std::unique_ptr<DxCore> dxcore;
}

#define dx_device	Khan::dxcore->GetDevice()
#define dx_context	Khan::dxcore->GetContext()
#define dx_rtv		Khan::dxcore->GetRTView()
#define dx_dsv		Khan::dxcore->GetDSView()
#define dx_dsstate	Khan::dxcore->GetDSState()
#define dx_rsstate	Khan::dxcore->GetRSState()
//#define dx_swapchain dxcore->swapchain

