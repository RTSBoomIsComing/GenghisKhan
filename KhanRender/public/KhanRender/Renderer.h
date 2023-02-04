#pragma once

#include "RenderingHub.h"

#include <memory>
#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace KhanRender
{
	class Renderer
	{
	public:
		Renderer(std::shared_ptr<RenderingHub> hub)
			:
			m_screenWidth(hub->GetScreenWidth()),
			m_screenHeight(hub->GetScreenHeight()),
			m_pDevice(hub->GetDevice()),
			m_pDeviceContext(hub->GetDeviceContext()) {}
		//virtual void Render() abstract;

	protected:
		UINT const& m_screenWidth;
		UINT const& m_screenHeight;
		ComPtr<ID3D11Device>& m_pDevice;
		ComPtr<ID3D11DeviceContext>& m_pDeviceContext;
	};
}

