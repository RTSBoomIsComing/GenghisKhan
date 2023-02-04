#include "pch.h"
#include "KhanDxFactories.h"
#include <KhanDx/KhanDxUtils.h>


KhanDx::D3D11DeviceWithContext KhanDx::CreateDeviceAndContext()
{
	static D3D11DeviceWithContext d3d_deviceWithContext;
	auto& [d3d_device, d3d_context] = d3d_deviceWithContext;
	if (d3d_device && d3d_context)
	{
		return d3d_deviceWithContext;
	}

	UINT CreateDeviceFlags{};
#if defined(DEBUG) || defined(_DEBUG)
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	KhanDx::ThrowIfFailed(::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, CreateDeviceFlags, nullptr, 0U,
		D3D11_SDK_VERSION, &d3d_device, nullptr,
		&d3d_context),
		"Failed to Create D3d Device and Context");

	return d3d_deviceWithContext;
}

ComPtr<IDXGISwapChain> KhanDx::CreateSwapChain(ComPtr<ID3D11Device> d3d_device, HWND hwnd)
{
	ComPtr<IDXGISwapChain> swapchain;

	ComPtr<IDXGIFactory> dxgi_factory;
	KhanDx::ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&dxgi_factory)),
		"Failed to Create DXGI Factory");

	DXGI_MODE_DESC buffer_desc{};
	buffer_desc.Width = 0U; // set as window client area 
	buffer_desc.Height = 0U; // set as window client area 
	buffer_desc.RefreshRate.Numerator = 0U;
	buffer_desc.RefreshRate.Denominator = 0U;
	buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapchain_desc{};
	swapchain_desc.BufferDesc = buffer_desc;
	swapchain_desc.SampleDesc.Count = 1U;
	swapchain_desc.SampleDesc.Quality = 0U;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.BufferCount = 2U;
	swapchain_desc.OutputWindow = hwnd;
	swapchain_desc.Windowed = true;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchain_desc.Flags = 0U;

	KhanDx::ThrowIfFailed(dxgi_factory->CreateSwapChain(d3d_device.Get(),
		&swapchain_desc, &swapchain),
		"Failed to Create SwapChain");

	return swapchain;
}

ComPtr<ID3D11RenderTargetView> KhanDx::CreateRenderTargetView(ComPtr<ID3D11Device> d3d_device, ComPtr<IDXGISwapChain> swapchain, UINT width, UINT height)
{
	ComPtr<ID3D11Resource> backBuffer;
	ThrowIfFailed(
		swapchain->GetBuffer(0u, IID_PPV_ARGS(&backBuffer)),
		"failed to get back buffer");

	ComPtr<ID3D11RenderTargetView> rtview;
	ThrowIfFailed(
		d3d_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &rtview),
		"failed to create back buffer");

	return rtview;
}

ComPtr<ID3D11DepthStencilView> KhanDx::CreateDepthStencilView(ComPtr<ID3D11Device> d3d_device, UINT width, UINT height)
{
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

	ComPtr<ID3D11DepthStencilView> dsview;
	ThrowIfFailed(
		d3d_device->CreateDepthStencilView(dsBuffer.Get(), &dsviewDesc, &dsview),
		"Failed to create depth stencil view");

	return dsview;
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
