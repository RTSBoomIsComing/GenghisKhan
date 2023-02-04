#pragma once

#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;
#include <string_view>
#include <tuple>

namespace KhanDx
{
	ComPtr<ID3D11RasterizerState> CreateRSState_Solid(ComPtr<ID3D11Device> d3d_device) noexcept;
	ComPtr<ID3D11RasterizerState> CreateRSState_Solid_NoCulling(ComPtr<ID3D11Device> d3d_device) noexcept;
	ComPtr<ID3D11RasterizerState> CreateRSState_WireFrame(ComPtr<ID3D11Device> d3d_device) noexcept;
	
	ComPtr<ID3D11DepthStencilState> CreateDSState_Default(ComPtr<ID3D11Device> d3d_device) noexcept;
	ComPtr<ID3D11BlendState> CreateBlendState_Alpha(ComPtr<ID3D11Device> d3d_device) noexcept;
	
	using D3D11VertexShaderAndInputLayout = std::tuple<ComPtr<ID3D11VertexShader>, ComPtr<ID3D11InputLayout>>;
	D3D11VertexShaderAndInputLayout CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device> d3d_device, std::string_view cso_name, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements);
	
	ComPtr<ID3D11Buffer> CreateVertexBuffer(ComPtr<ID3D11Device> d3d_device, const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3D11Buffer> CreateIndexBuffer(ComPtr<ID3D11Device> d3d_device, const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3DBlob> CreateShaderBlob(std::string_view cso_name);
	ComPtr<ID3D11PixelShader> CreatePixelShader(ComPtr<ID3D11Device> d3d_device, std::string_view fileName);
	ComPtr<ID3D11VertexShader> CreateVertexShader(ComPtr<ID3D11Device> d3d_device, ComPtr<ID3DBlob> pShaderBlob) noexcept;
	ComPtr<ID3D11InputLayout> CreateInputLayout(ComPtr<ID3D11Device> d3d_device, ComPtr<ID3DBlob> pShaderBlob, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements) noexcept;

	template<typename T, UINT count>
	ComPtr<ID3D11Buffer> CreateDynamicCBuffer(ComPtr<ID3D11Device> d3d_device) noexcept
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

		d3d_device->CreateBuffer(&desc, nullptr, &cbuffer);

		return cbuffer;
	}

}