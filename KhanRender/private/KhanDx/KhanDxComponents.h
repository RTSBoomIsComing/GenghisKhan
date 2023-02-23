#pragma once

#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;
#include <string_view>
#include <tuple>
#include <filesystem>

struct aiTexture;

namespace KhanDx
{
	ComPtr<ID3D11RasterizerState> CreateRasterizerState_Solid(ID3D11Device* pDevice) noexcept;
	ComPtr<ID3D11RasterizerState> CreateRasterizerState_Solid_NoCulling(ID3D11Device* pDevice) noexcept;
	ComPtr<ID3D11RasterizerState> CreateRasterizerState_WireFrame(ID3D11Device* pDevice) noexcept;
	ComPtr<ID3D11SamplerState>	  CreateSamplerState_Basic(ID3D11Device* pDevice) noexcept;
	
	ComPtr<ID3D11DepthStencilState> CreateDepthStencilState_Default(ID3D11Device* pDevice) noexcept;
	ComPtr<ID3D11BlendState> CreateBlendState_Alpha(ID3D11Device* pDevice) noexcept;
	
	
	ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* pDevice, const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3D11Buffer> CreateIndexBuffer(ID3D11Device* pDevice, const void* pSysMem, UINT byteWidth) noexcept;

	ComPtr<ID3DBlob> CreateShaderBlob(std::string_view shaderName);
	ComPtr<ID3D11PixelShader> CreatePixelShader(ID3D11Device* pDevice, std::string_view shaderName);
	ComPtr<ID3D11VertexShader> CreateVertexShader(ID3D11Device* pDevice, ComPtr<ID3DBlob> pBlob) noexcept;
	ComPtr<ID3D11InputLayout> CreateInputLayout(ID3D11Device* pDevice, ComPtr<ID3DBlob> pBlob, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements) noexcept;

	ComPtr<ID3D11ShaderResourceView> CreateSRV_StructBuf(ID3D11Device* pDevice, ComPtr<ID3D11Resource> pBuf, UINT firstElement, UINT numElements) noexcept;

	ComPtr<ID3D11ShaderResourceView> CreateSRV_Texture2D(ID3D11Device* pDevice, std::filesystem::path filePath);
	ComPtr<ID3D11ShaderResourceView> CreateSRV_Texture2D(ID3D11Device* pDevice,  const aiTexture* pAiTexture);
	ComPtr<ID3D11ShaderResourceView> CreateSRV_Texture2D(ID3D11Device* pDevice, const uint8_t* pImageData, int width, int height, int channels);

	template<typename T>
	ComPtr<ID3D11Buffer> CreateDynConstBuf(ID3D11Device* pDevice, unsigned int maxNumElements) noexcept
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

	ComPtr<ID3D11Buffer> CreateDynStructBuf(ID3D11Device* pDevice, unsigned int structureByteStride, unsigned int numElements) noexcept;

}