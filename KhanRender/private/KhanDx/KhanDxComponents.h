#pragma once

#include <d3d11.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;
#include <string_view>
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

	ComPtr<ID3D11ShaderResourceView> CreateSRV_StructBuf(ID3D11Device* pDevice, ID3D11Resource* pBuf, UINT firstElement, UINT numElements) noexcept;
	ComPtr<ID3D11ShaderResourceView> CreateSRV_StaticStructBuf(ID3D11Device* pDevice, const void* pSysMem, int structureByteStride, uint32_t numElements) noexcept;

	ComPtr<ID3D11ShaderResourceView> CreateSRV_Texture2D(ID3D11Device* pDevice, std::filesystem::path filePath);
	ComPtr<ID3D11ShaderResourceView> CreateSRV_Texture2D(ID3D11Device* pDevice,  const aiTexture* pAiTexture);
	ComPtr<ID3D11ShaderResourceView> CreateSRV_Texture2D(ID3D11Device* pDevice, const uint8_t* pImageData, int width, int height, int channels);

	ComPtr<ID3D11Buffer> CreateDynConstBuf(ID3D11Device* pDevice, unsigned int structureByteStride, unsigned int numElements = D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT) noexcept;
	ComPtr<ID3D11Buffer> CreateDynStructBuf(ID3D11Device* pDevice, unsigned int structureByteStride, unsigned int numElements) noexcept;

}