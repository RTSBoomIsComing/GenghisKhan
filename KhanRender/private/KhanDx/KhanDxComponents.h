#pragma once

#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;
#include <string_view>
#include <tuple>

namespace KhanDx
{
	ComPtr<ID3D11RasterizerState> CreateRasterizerState_Solid(ComPtr<ID3D11Device> pDevice) noexcept;
	ComPtr<ID3D11RasterizerState> CreateRasterizerState_Solid_NoCulling(ComPtr<ID3D11Device> pDevice) noexcept;
	ComPtr<ID3D11RasterizerState> CreateRasterizerState_WireFrame(ComPtr<ID3D11Device> pDevice) noexcept;
	
	ComPtr<ID3D11DepthStencilState> CreateDepthStencilState_Default(ComPtr<ID3D11Device> pDevice) noexcept;
	ComPtr<ID3D11BlendState> CreateBlendState_Alpha(ComPtr<ID3D11Device> pDevice) noexcept;
	
	
	ComPtr<ID3D11Buffer> CreateVertexBuffer(ComPtr<ID3D11Device> pDevice, const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3D11Buffer> CreateIndexBuffer(ComPtr<ID3D11Device> pDevice, const void* pSysMem, UINT byteWidth) noexcept;

	ComPtr<ID3DBlob> CreateShaderBlob(std::string_view shaderName);
	ComPtr<ID3D11PixelShader> CreatePixelShader(ComPtr<ID3D11Device> pDevice, std::string_view shaderName);
	ComPtr<ID3D11VertexShader> CreateVertexShader(ComPtr<ID3D11Device> pDevice, ComPtr<ID3DBlob> pBlob) noexcept;
	ComPtr<ID3D11InputLayout> CreateInputLayout(ComPtr<ID3D11Device> pDevice, ComPtr<ID3DBlob> pBlob, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements) noexcept;

	ComPtr<ID3D11ShaderResourceView> CreateSRV_StructBuf(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11Resource> pBuf, UINT firstElement, UINT numElements) noexcept;

	template<typename T>
	ComPtr<ID3D11Buffer> CreateDynConstBuf(ComPtr<ID3D11Device> pDevice, UINT maxNumElements) noexcept
	{
		D3D11_BUFFER_DESC bufDesc{};
		bufDesc.ByteWidth = sizeof(T) * maxNumElements;
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufDesc.MiscFlags = 0U;
		bufDesc.StructureByteStride = 0U;

		ComPtr<ID3D11Buffer> buf;
		pDevice->CreateBuffer(&bufDesc, nullptr, &buf);

		return buf;
	}

	template<typename T>
	ComPtr<ID3D11Buffer> CreateDynStructBuf(ComPtr<ID3D11Device> pDevice, UINT numElements) noexcept
	{
		D3D11_BUFFER_DESC bufDesc{};
		bufDesc.ByteWidth = sizeof(T) * numElements;
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufDesc.StructureByteStride = sizeof(T);


		ComPtr<ID3D11Buffer> buf;
		pDevice->CreateBuffer(&bufDesc, nullptr, &buf);

		return buf;
	}

}