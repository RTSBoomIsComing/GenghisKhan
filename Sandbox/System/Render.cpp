#include "pch.h"
#include "Render.h"
#include <Khan/DxCore.h>
#include <Khan/Dx/Renderer.h>

void Render::Clear() noexcept
{
	float bg_color[]{ 0.f, 1.f, 1.f, 1.f };
	
	dx_context->OMSetRenderTargets(1u, dx_rtv.GetAddressOf(), dx_dsv.Get());
	dx_context->ClearRenderTargetView(dx_rtv.Get(), bg_color);
	dx_context->ClearDepthStencilView(dx_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
