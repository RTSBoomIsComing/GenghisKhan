#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <windef.h>
#include <memory>
#include "Window/Window.h"
#include <cassert>

using Microsoft::WRL::ComPtr;

namespace Khan
{
	class DxCore
	{
		DxCore(HWND hwnd, int width, int height) noexcept;
	public:
		void SwapBuffers() noexcept;
		void ResizeBackBuffers(UINT width, UINT height) noexcept;

	private:
		void InitializeDepthStencil(UINT width, UINT height) noexcept;
		void InitializeRenderTarget() noexcept;
		void InitializeViewport(float width, float height) noexcept;

	private:
		ComPtr<ID3D11Device>			m_device;
		ComPtr<IDXGISwapChain>			m_swapchain;
		ComPtr<ID3D11DeviceContext>		m_context;
		ComPtr<ID3D11RenderTargetView>	m_rtview;
		ComPtr<ID3D11DepthStencilView>	m_dsview;
		ComPtr<ID3D11DepthStencilState> m_dsstate;


	public:
		ComPtr<ID3D11Device>& GetDevice() noexcept { return m_device; }
		ComPtr<ID3D11DeviceContext>& GetContext() noexcept { return m_context; }
		ComPtr<ID3D11RenderTargetView>& GetRTView() noexcept { return m_rtview; }
		ComPtr<ID3D11DepthStencilView>& GetDSView() noexcept { return m_dsview; }
		ComPtr<ID3D11DepthStencilState>& GetDSState() noexcept { return m_dsstate; }

	// Make Singleton
	private:
		static inline std::unique_ptr<DxCore> m_instance{};
	
	public:
		static void Initialize(HWND hwnd, int width, int height)
		{
			assert(!m_instance&& "dxcore already initialized");
			m_instance = std::unique_ptr<DxCore>(new DxCore(hwnd, width, height));
		}
		static std::unique_ptr<Khan::DxCore>& GetInstance()
		{
			assert(m_instance && "need to initialize");
			return m_instance;
		}
	};
}

#define dxcore		Khan::DxCore::GetInstance()
#define dx_device	dxcore->GetDevice()
#define dx_context	dxcore->GetContext()
#define dx_rtv		dxcore->GetRTView()
#define dx_dsv		dxcore->GetDSView()
#define dx_dsstate	dxcore->GetDSState()
//#define dx_swapchain dxcore->swapchain

