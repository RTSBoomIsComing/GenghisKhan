#pragma once
#include <memory>
#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace KhanRender
{
	class Renderer
	{
	public:
		Renderer() noexcept;

		ComPtr<ID3D11Device> m_pDevice;
		ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	};
}

