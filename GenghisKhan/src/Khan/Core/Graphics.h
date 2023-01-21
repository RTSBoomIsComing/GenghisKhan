#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <windef.h>

using Microsoft::WRL::ComPtr;

namespace Khan
{
	class Graphics
	{
	public:
		Graphics(HWND hwnd, int width, int height) noexcept;
		~Graphics() noexcept;
		void SwapBuffers() noexcept;
	public:

		ComPtr<ID3D11Device> m_device;
		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID3D11DeviceContext> m_context;
		D3D_FEATURE_LEVEL m_feature_level{};
	};
}

