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
		Renderer(std::shared_ptr<RenderingHub> pHub)
			:
			m_screenWidth(pHub->GetScreenWidth()),
			m_screenHeight(pHub->GetScreenHeight()),
			m_pDevice(pHub->GetDevice()),
			m_pDeviceContext(pHub->GetDeviceContext()) {}
		//virtual void Render() abstract;
		//void OnResize();

	protected:
		UINT const& m_screenWidth;
		UINT const& m_screenHeight;
		ComPtr<ID3D11Device>& m_pDevice;
		ComPtr<ID3D11DeviceContext>& m_pDeviceContext;
	};
}

