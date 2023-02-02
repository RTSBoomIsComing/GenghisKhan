#include "pch.h"
#include "KhanDxFactories.h"
#include <KhanDx/KhanDxUtils.h>

void KhanDx::CreateRenderTarget(ID3D11Device* d3d_device, IDXGISwapChain* swapchain, ID3D11RenderTargetView** rtview, UINT width, UINT height)
{
	ComPtr<ID3D11Resource> backBuffer;
	ThrowIfFailed(
		swapchain->GetBuffer(0u, IID_PPV_ARGS(&backBuffer)),
		"failed to get back buffer");

	ThrowIfFailed(
		d3d_device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtview),
		"failed to create back buffer");
}

void KhanDx::CreateDepthStencilStateAndView(ID3D11Device* d3d_device, ID3D11DepthStencilState** dsstate, ID3D11DepthStencilView** dsview, UINT width, UINT height)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
	ThrowIfFailed(
		d3d_device->CreateDepthStencilState(&dsDesc, dsstate),
		"Failed to create depth stencil state");

	ComPtr<ID3D11Texture2D> dsBuffer;
	D3D11_TEXTURE2D_DESC dsBufferDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D32_FLOAT, width, height);
	dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ThrowIfFailed(
		d3d_device->CreateTexture2D(&dsBufferDesc, nullptr, &dsBuffer),
		"Failed to create texture of depth stencil");

	D3D11_DEPTH_STENCIL_VIEW_DESC dsviewDesc{};
	dsviewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsviewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsviewDesc.Texture2D.MipSlice = 0u;

	ThrowIfFailed(
		d3d_device->CreateDepthStencilView(dsBuffer.Get(), &dsviewDesc, dsview),
		"Failed to create depth stencil view");
}

D3D11_VIEWPORT KhanDx::CreateDefaultViewport(float width, float height) noexcept
{
	D3D11_VIEWPORT viewport{};
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	return viewport;
}
