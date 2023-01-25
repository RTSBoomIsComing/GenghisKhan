#include "pch.h"
#include "DxCore.h"
#include "Dx/DxUtility.h"
#include "Dx/DxFunction.h"

namespace Khan
{
	DxCore::DxCore(HWND hwnd, int width, int height) noexcept
	{
		UINT createDeviceFlags{};
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
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

		// create device and front/back buffers, and swap chain and rendering context
		Khan::ThrowIfFailed(
			::D3D11CreateDeviceAndSwapChain(
				nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
				nullptr, 0u, D3D11_SDK_VERSION,
				&swapchain_desc, &m_swapchain, &m_device, nullptr, &m_context),
			"failed to create device and swapchain");

		this->InitializeRenderTarget();
		this->InitializeDepthStencil(width, height);
		this->InitializeViewport(static_cast<float>(width), static_cast<float>(height));
	}

	void DxCore::InitializeDepthStencil(UINT width, UINT height) noexcept
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
		Khan::ThrowIfFailed(
			m_device->CreateDepthStencilState(&dsDesc, &m_dsstate),
			"failed to create depth stencil state");

		m_context->OMSetDepthStencilState(m_dsstate.Get(), 1u);

		ComPtr<ID3D11Texture2D> dsBuffer;
		D3D11_TEXTURE2D_DESC dsBufferDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D32_FLOAT, width, height);
		dsBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		Khan::ThrowIfFailed(
			m_device->CreateTexture2D(&dsBufferDesc, nullptr, &dsBuffer),
			"failed to create texture of depth stencil");

		D3D11_DEPTH_STENCIL_VIEW_DESC dsviewDesc{};
		dsviewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsviewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsviewDesc.Texture2D.MipSlice = 0u;

		Khan::ThrowIfFailed(
			m_device->CreateDepthStencilView(dsBuffer.Get(), &dsviewDesc, &m_dsview),
			"failed to create depth stencil view");
	}

	void DxCore::InitializeRenderTarget() noexcept
	{
		ComPtr<ID3D11Resource> backBuffer;
		ThrowIfFailed(
			m_swapchain->GetBuffer(0u, IID_PPV_ARGS(&backBuffer)),
			"failed to get back buffer");

		ThrowIfFailed(
			m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtview),
			"failed to create back buffer");
	}

	void DxCore::InitializeViewport(float width, float height) noexcept
	{
		D3D11_VIEWPORT viewport{};
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		m_context->RSSetViewports(1u, &viewport);
	}

	void DxCore::SwapBuffers() noexcept
	{
		m_swapchain->Present(1u, 0u);
	}

	void DxCore::ResizeBackBuffers(UINT width, UINT height) noexcept
	{
		m_context->OMSetRenderTargets(0u, nullptr, nullptr);
		m_context->Flush();
		m_rtview = nullptr;
		m_dsview = nullptr;

		m_swapchain->ResizeBuffers(0u, 0u, 0u, DXGI_FORMAT_UNKNOWN, 0u);

		this->InitializeRenderTarget();
		this->InitializeDepthStencil(width, height);
		this->InitializeViewport(static_cast<float>(width), static_cast<float>(height));

		//Reset camera's aspect ratio based on backBufferWidth/backBufferHeight
	}
}