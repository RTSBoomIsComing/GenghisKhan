#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
// TODO: class RenderSystem; // move core members to rendersystem, then core is including rendersystem.

namespace Khan
{
	class DxCore
	{
	public:
		DxCore(HWND hwnd, int  width, int  height) noexcept;
	public:
		void RenderStart() noexcept;
		void RenderEnd() noexcept;
		void OnResizeWindow(UINT width, UINT height) noexcept;
	private:
		ComPtr<ID3D11Device> m_device;
		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID3D11DeviceContext> m_context;
		ComPtr<ID3D11RenderTargetView> m_RTV;
		ComPtr<ID3D11DepthStencilView> m_DSV;

		D3D_FEATURE_LEVEL m_feature_level;
	private:
		void CreateDeviceAndSwapChain(HWND hwnd, int  width, int  height);
		void CreateRenderTargetView();
		void CreateDepthStencilView(int wnd_width, int wnd_height);

	public:
		ComPtr<ID3D11Device>& GetDevice() noexcept { return m_device; }
		ComPtr<ID3D11DeviceContext>& GetContext() noexcept { return m_context; }
	};
}
