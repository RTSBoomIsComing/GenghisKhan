#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <windef.h>

using Microsoft::WRL::ComPtr;

namespace Khan
{
	class DxCore
	{
	public:
		DxCore(HWND hwnd, int width, int height) noexcept;
		~DxCore() noexcept;
		void SwapBuffers() noexcept;
	public:

		ComPtr<ID3D11Device> m_device;
		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID3D11DeviceContext> m_context;
		D3D_FEATURE_LEVEL m_feature_level{};
	};
}

