#pragma once
#include <d3d11.h>
#include <tuple>
#include <wrl.h>
using Microsoft::WRL::ComPtr;


namespace KhanDx
{
	using D3D11DeviceWithContext = std::tuple<ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>>;
	D3D11DeviceWithContext CreateDeviceAndContext();
	ComPtr<IDXGISwapChain> CreateSwapChain(ComPtr<ID3D11Device> d3d_device, HWND hwnd);
	ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(ComPtr<ID3D11Device> d3d_device, ComPtr<IDXGISwapChain> swapchain, UINT width, UINT height);
	ComPtr< ID3D11DepthStencilView> CreateDepthStencilView(ComPtr<ID3D11Device> d3d_device, UINT width, UINT height);
	D3D11_VIEWPORT CreateDefaultViewport(float width, float height) noexcept;
}