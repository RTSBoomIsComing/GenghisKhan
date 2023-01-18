#include "pch.h"
#include "Graphics.h"
#include "DxUtility.h"
#include <chrono>
namespace Khan {
	Khan::Graphics::Graphics(HWND hwnd, int width, int height) noexcept
	{
		CreateDeviceAndSwapChain(hwnd, width, height);
		CreateRenderTargetView();
		CreateDepthStencilView(width, height);

		// bind depth stencil view to OM
		m_context->OMSetRenderTargets(1U, m_RTV.GetAddressOf(), m_DSV.Get());
	}
	void Graphics::RenderStart() noexcept
	{
		using namespace std::chrono;
		float bg_color[]{ 0.f, 0.f, 0.f, 1.f };
		static steady_clock::time_point start = steady_clock::now();
		{
			steady_clock::time_point current = steady_clock::now();
			duration<float> elapsed_time = current - start;
			float c = 0.5f + 0.5 * sin(elapsed_time.count());
			bg_color[1] = c;
			bg_color[2] = c;
		}
		m_context->ClearRenderTargetView(m_RTV.Get(), bg_color);
		m_context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}
	void Graphics::RenderEnd() noexcept
	{
		m_swapChain->Present(0u, 0u);
	}
	void Graphics::ResizeBackBuffers(UINT width, UINT height) noexcept
	{
		m_swapChain->ResizeBuffers(0u, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0u);
		CreateRenderTargetView();
	}
	void Graphics::CreateDeviceAndSwapChain(HWND hwnd, int window_width, int window_height)
	{
		DXGI_SWAP_CHAIN_DESC sc_desc{};
		{
			sc_desc.BufferDesc.Width = window_width;
			sc_desc.BufferDesc.Height = window_height;
			sc_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sc_desc.BufferDesc.RefreshRate.Numerator = 0U;
			sc_desc.BufferDesc.RefreshRate.Denominator = 0U;
			sc_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			sc_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sc_desc.SampleDesc.Count = 1U;
			sc_desc.SampleDesc.Quality = 0U;
			sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sc_desc.BufferCount = 1U;
			sc_desc.OutputWindow = hwnd;
			sc_desc.Windowed = true;
			sc_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			sc_desc.Flags = 0U;
		}

		D3D_FEATURE_LEVEL feature_levels[]
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_9_3
		};

		UINT createDeviceFlags{};
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// create device and front/back buffers, and swap chain and rendering context
		ThrowIfFailed(
			D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				createDeviceFlags,
				feature_levels,
				ARRAYSIZE(feature_levels),
				D3D11_SDK_VERSION,
				&sc_desc,
				&m_swapChain,
				&m_device,
				&m_feature_level,
				&m_context),
			"failed to create device and swapchain");
	}
	void Graphics::CreateRenderTargetView()
	{
		// gain access to texture subresource in swap chain (back buffer)
		ComPtr<ID3D11Resource> backBuffer;
		ThrowIfFailed(
			m_swapChain->GetBuffer(0u, IID_PPV_ARGS(&backBuffer)),
			"failed to get back buffer");

		ThrowIfFailed(
			m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_RTV),
			"failed to create back buffer");
	}
	void Graphics::CreateDepthStencilView(int window_width, int window_height)
	{
		// create depth stencil state
		ComPtr<ID3D11DepthStencilState> ds_state;
		D3D11_DEPTH_STENCIL_DESC ds_desc{};
		{
			ds_desc.DepthEnable = true;
			ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			ds_desc.DepthFunc = D3D11_COMPARISON_LESS;
		}

		ThrowIfFailed(
			m_device->CreateDepthStencilState(&ds_desc, &ds_state),
			"failed to create depth stencil state");

		m_context->OMSetDepthStencilState(ds_state.Get(), 1u);

		ComPtr<ID3D11Texture2D> ds_texture;
		D3D11_TEXTURE2D_DESC texture_desc{};
		{
			texture_desc.Width = window_width;
			texture_desc.Height = window_height;
			texture_desc.MipLevels = 1u;
			texture_desc.ArraySize = 1u;
			texture_desc.Format = DXGI_FORMAT_D32_FLOAT; //about D32, D := depth, special format for depth
			texture_desc.SampleDesc.Count = 1u;
			texture_desc.SampleDesc.Quality = 0u;
			texture_desc.Usage = D3D11_USAGE_DEFAULT;
			texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		}
		ThrowIfFailed(
			m_device->CreateTexture2D(&texture_desc, nullptr, &ds_texture),
			"failed to create texture of depth stencil");

		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc{};
		{
			dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Texture2D.MipSlice = 0u;
		}
		ThrowIfFailed(
			m_device->CreateDepthStencilView(ds_texture.Get(), &dsv_desc, &m_DSV),
			"failed to create depth stencil view");
	}
}
