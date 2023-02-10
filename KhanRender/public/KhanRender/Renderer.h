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
		Renderer(const Renderer& other) noexcept
			:
			m_pDevice(other.m_pDevice),
			m_pDeviceContext(other.m_pDeviceContext)
		{}

		ComPtr<ID3D11Device> m_pDevice;
		ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	};
}

