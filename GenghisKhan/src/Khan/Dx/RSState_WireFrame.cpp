#include "pch.h"
#include "RSState_WireFrame.h"

#include "DxCore.h"


namespace Khan
{
	RSState_WireFrame::RSState_WireFrame() noexcept
	{
		assert(dxcore && "dxcore has to be initialized.");

		D3D11_RASTERIZER_DESC desc{};
		desc.FillMode = D3D11_FILL_WIREFRAME;	//D3D11_FILL_SOLID;	// D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;		// D3D11_CULL_BACK;

		Khan::ThrowIfFailed(dx_device->CreateRasterizerState(&desc, &rsstate),
			"failed to create RS state");
	}
	void RSState_WireFrame::Bind() noexcept
	{
		dx_context->RSSetState(rsstate.Get());
	}
}