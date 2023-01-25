#include "pch.h"
#include "DxFunction.h"
#include "Dx/DxUtility.h"
#include "DxCore.h"
namespace Khan
{
	void CreateDSView(int width, int height, ID3D11DepthStencilView** ppDsview)
	{
		// create depth stencil state
		ComPtr<ID3D11DepthStencilState> ds_state;
		D3D11_DEPTH_STENCIL_DESC ds_desc{};
		{
			ds_desc.DepthEnable = true;
			ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			ds_desc.DepthFunc = D3D11_COMPARISON_LESS;
		}

		Khan::ThrowIfFailed(
			dx_device->CreateDepthStencilState(&ds_desc, &ds_state),
			"failed to create depth stencil state");

		dx_context->OMSetDepthStencilState(ds_state.Get(), 1u);

		ComPtr<ID3D11Texture2D> ds_texture;
		D3D11_TEXTURE2D_DESC texture_desc{};
		{
			texture_desc.Width = width;
			texture_desc.Height = height;
			texture_desc.MipLevels = 1u;
			texture_desc.ArraySize = 1u;
			texture_desc.Format = DXGI_FORMAT_D32_FLOAT; //about D32, D := depth, special format for depth
			texture_desc.SampleDesc.Count = 1u;
			texture_desc.SampleDesc.Quality = 0u;
			texture_desc.Usage = D3D11_USAGE_DEFAULT;
			texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		}
		Khan::ThrowIfFailed(
			dx_device->CreateTexture2D(&texture_desc, nullptr, &ds_texture),
			"failed to create texture of depth stencil");

		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc{};
		{
			dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Texture2D.MipSlice = 0u;
		}
		Khan::ThrowIfFailed(
			dx_device->CreateDepthStencilView(ds_texture.Get(), &dsv_desc, ppDsview),
			"failed to create depth stencil view");
	}
}
