#include "pch.h"
#include "RenderingHub.h"
#include "KhanDx/KhanDxUtils.h"
#include "KhanDx/KhanDxFactories.h"

KhanRender::RenderingHub::RenderingHub(HWND hwnd, UINT width, UINT height)
{
	rt_width = width;
	rt_height = height;
	this->CreateDeviceAndContext();
	this->CreateSwapChain(hwnd, width, height);
	KhanDx::CreateRenderTarget(d3d_device.Get(), m_swapchain.Get(), &rtview, width, height);

	KhanDx::CreateDepthStencilStateAndView(d3d_device.Get(), &dsstate, &dsview, width, height);
	d3d_context->OMSetDepthStencilState(dsstate.Get(), 1u);

	viewport = KhanDx::CreateDefaultViewport(static_cast<float>(width), static_cast<float>(height));
	d3d_context->RSSetViewports(1u, &viewport);
}

void KhanRender::RenderingHub::CreateDeviceAndContext()
{
	if (d3d_device && d3d_context) return;

	UINT CreateDeviceFlags{};
#if defined(DEBUG) || defined(_DEBUG)
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	KhanDx::ThrowIfFailed(::D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, CreateDeviceFlags, nullptr, 0U,
		D3D11_SDK_VERSION, &d3d_device, nullptr,
		&d3d_context),
		"Failed to Create D3d Device and Context");
}

void KhanRender::RenderingHub::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
	ComPtr<IDXGIFactory> dxgi_factory;
	KhanDx::ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(&dxgi_factory)),
		"Failed to Create DXGI Factory");

	DXGI_MODE_DESC buffer_desc{};
	buffer_desc.Width = width;	// set as window client area 
	buffer_desc.Height = height;	// set as window client area 
	buffer_desc.RefreshRate.Numerator = 0u;
	buffer_desc.RefreshRate.Denominator = 0u;
	buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapchain_desc{};
	swapchain_desc.BufferDesc = buffer_desc;
	swapchain_desc.SampleDesc.Count = 1u;
	swapchain_desc.SampleDesc.Quality = 0u;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.BufferCount = 2u;
	swapchain_desc.OutputWindow = hwnd;
	swapchain_desc.Windowed = true;
	swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchain_desc.Flags = 0u;

	KhanDx::ThrowIfFailed(dxgi_factory->CreateSwapChain(d3d_device.Get(),
		&swapchain_desc, &m_swapchain),
		"Failed to Create SwapChain");
}

void KhanRender::RenderingHub::ResizeRenderTarget(UINT width, UINT height) noexcept
{
	d3d_context->OMSetRenderTargets(0u, nullptr, nullptr);
	d3d_context->Flush();
	rtview = nullptr;
	dsview = nullptr;

	m_swapchain->ResizeBuffers(0u, 0u, 0u, DXGI_FORMAT_UNKNOWN, 0u);

	KhanDx::CreateRenderTarget(d3d_device.Get(), m_swapchain.Get(), &rtview, width, height);
	KhanDx::CreateDepthStencilStateAndView(d3d_device.Get(), &dsstate, &dsview, width, height);
	viewport = KhanDx::CreateDefaultViewport(static_cast<float>(width), static_cast<float>(height));
	d3d_context->RSSetViewports(1u, &viewport);

	//Reset camera's aspect ratio based on backBufferWidth/backBufferHeight
}

void KhanRender::RenderingHub::RenderBegin() noexcept
{

	float bg_color[]{ 0.0F, 0.0F, 0.0F, 1.0F };

	d3d_context->OMSetRenderTargets(1u, rtview.GetAddressOf(), dsview.Get());
	d3d_context->ClearRenderTargetView(rtview.Get(), bg_color);
	d3d_context->ClearDepthStencilView(dsview.Get(), D3D11_CLEAR_DEPTH, 1.0F, 0U);
}

void KhanRender::RenderingHub::RenderEnd() noexcept
{
	m_swapchain->Present(1U, 0U);
}
