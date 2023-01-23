#include "pch.h"
#include "DxCore.h"
#include "Common/DxUtility.h"
namespace Khan
{
	DxCore::DxCore(HWND hwnd, int width, int height) noexcept
	{
		DXGI_SWAP_CHAIN_DESC sc_desc{};
		{
			sc_desc.BufferDesc.Width = width;
			sc_desc.BufferDesc.Height = height;
			sc_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sc_desc.BufferDesc.RefreshRate.Numerator = 0u;
			sc_desc.BufferDesc.RefreshRate.Denominator = 0u;
			sc_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			sc_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sc_desc.SampleDesc.Count = 1u;
			sc_desc.SampleDesc.Quality = 0u;
			sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sc_desc.BufferCount = 1u;
			sc_desc.OutputWindow = hwnd;
			sc_desc.Windowed = true;
			sc_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			sc_desc.Flags = 0u;
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
			::D3D11CreateDeviceAndSwapChain(
				nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
				feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION,
				&sc_desc, &m_swapChain, &m_device, &m_feature_level, &m_context),
			"failed to create device and swapchain");
	}

	DxCore::~DxCore() noexcept
	{
	}

	void DxCore::SwapBuffers() noexcept
	{
		m_swapChain->Present(1u, 0u);
	}
}