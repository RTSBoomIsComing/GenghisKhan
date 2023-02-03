#pragma once
#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace KhanDx
{
	ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapchain, UINT width, UINT height);
	void CreateDepthStencilStateAndView(ID3D11Device* d3d_device, ID3D11DepthStencilState** dsstate, ID3D11DepthStencilView** dsview, UINT width, UINT height);
	D3D11_VIEWPORT CreateDefaultViewport(float width, float height) noexcept;
}