#pragma once

#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;
#include <string_view>

namespace KhanDx
{
	ComPtr<ID3D11RasterizerState> CreateRSState_Solid(ID3D11Device* d3d_device) noexcept;
	ComPtr<ID3D11RasterizerState> CreateRSState_Solid_NoCulling(ID3D11Device* d3d_device) noexcept;
	ComPtr<ID3D11RasterizerState> CreateRSState_WireFrame(ID3D11Device* d3d_device) noexcept;
	ComPtr<ID3D11DepthStencilState> CreateDSState_Default(ID3D11Device* d3d_device) noexcept;
	ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* d3d_device, const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3D11Buffer> CreateIndexBuffer(ID3D11Device* d3d_device, const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3D11BlendState> CreateBlendState_Alpha(ID3D11Device* d3d_device) noexcept;
	ComPtr<ID3DBlob> CreateShaderBlob(std::string_view fileName);
	ComPtr<ID3D11PixelShader> CreatePixelShader(ID3D11Device* d3d_device, std::string_view fileName);
	ComPtr<ID3D11VertexShader> CreateVertexShader(ID3D11Device* d3d_device, ID3DBlob* pShaderBlob) noexcept;
	ComPtr<ID3D11InputLayout> CreateInputLayout(ID3D11Device* d3d_device, ID3DBlob* pShaderBlob, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements) noexcept;

	template<typename T, UINT count>
	ComPtr<ID3D11Buffer> CreateDynamicCBuffer(ID3D11Device* d3d_device) noexcept
	{
		ComPtr<ID3D11Buffer> cbuffer;
		D3D11_BUFFER_DESC desc{};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0u;
		desc.ByteWidth = sizeof(T) * count;

		T tempSysMem[count]{};

		D3D11_SUBRESOURCE_DATA s_data{};
		s_data.pSysMem = &tempSysMem;

		d3d_device->CreateBuffer(&desc, &s_data, &cbuffer);

		return cbuffer;
	}
}