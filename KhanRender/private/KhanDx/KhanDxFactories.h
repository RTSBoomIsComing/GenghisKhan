#pragma once
#include <d3d11.h>
#include <tuple>
#include <wrl.h>
using Microsoft::WRL::ComPtr;


namespace KhanDx
{
	std::tuple<ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>> CreateDeviceAndContext();
	ComPtr<IDXGISwapChain> CreateSwapChain(ID3D11Device* d3d_device, HWND hwnd);
	ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(ID3D11Device* d3d_device, IDXGISwapChain* swapchain, UINT width, UINT height);
	ComPtr< ID3D11DepthStencilView> CreateDepthStencilView(ID3D11Device* d3d_device, UINT width, UINT height);
	D3D11_VIEWPORT CreateDefaultViewport(float width, float height) noexcept;
}