#include "pch.h"
#include "KhanDxComponents.h"
#include "KhanDx/KhanDxUtils.h"
#include <KhanTools/Log.h>
// standard libraries
#include <format>

// additional dependencies
#include <stb_image.h>


ComPtr<ID3D11RasterizerState> KhanDx::CreateRasterizerState_Solid(ID3D11Device* pDevice) noexcept
{
	ComPtr<ID3D11RasterizerState> rsstate;
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;

	ThrowIfFailed(pDevice->CreateRasterizerState(&desc, &rsstate),
		"failed to create RS state");

	return rsstate;
}

ComPtr<ID3D11RasterizerState> KhanDx::CreateRasterizerState_Solid_NoCulling(ID3D11Device* pDevice) noexcept
{
	ComPtr<ID3D11RasterizerState> rsstate;
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;

	ThrowIfFailed(pDevice->CreateRasterizerState(&desc, &rsstate),
		"failed to create RS state");

	return rsstate;
}

ComPtr<ID3D11RasterizerState> KhanDx::CreateRasterizerState_WireFrame(ID3D11Device* pDevice) noexcept
{
	ComPtr<ID3D11RasterizerState> rsstate;
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;

	ThrowIfFailed(pDevice->CreateRasterizerState(&desc, &rsstate),
		"Failed to Create Rasterizer State");

	return rsstate;
}

ComPtr<ID3D11SamplerState> KhanDx::CreateSamplerState_Basic(ID3D11Device* pDevice) noexcept
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;//D3D11_TEXTURE_ADDRESS_WRAP; D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;


	ComPtr<ID3D11SamplerState> pSamplerState;
	HRESULT hr = pDevice->CreateSamplerState(&samplerDesc, &pSamplerState);
	ThrowIfFailed(hr, "Failed to crate sampler state");
	return pSamplerState;
}

ComPtr<ID3D11DepthStencilState> KhanDx::CreateDepthStencilState_Default(ID3D11Device* pDevice) noexcept
{
	ComPtr<ID3D11DepthStencilState> dsstate;
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
	ThrowIfFailed(
		pDevice->CreateDepthStencilState(&dsDesc, &dsstate),
		"Failed to create depth stencil state");

	return dsstate;
}

ComPtr<ID3D11Buffer> KhanDx::CreateVertexBuffer(ID3D11Device* pDevice, const void* pSysMem, UINT byteWidth) noexcept
{
	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitVertexData{};
	InitVertexData.pSysMem = pSysMem;
	InitVertexData.SysMemPitch = 0U; // not used 
	InitVertexData.SysMemSlicePitch = 0U; // not used

	// Fill in a buffer description.
	D3D11_BUFFER_DESC vertexbufferDesc{};
	vertexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexbufferDesc.ByteWidth = byteWidth;
	vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexbufferDesc.CPUAccessFlags = 0U;
	vertexbufferDesc.MiscFlags = 0U;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ThrowIfFailed(pDevice->CreateBuffer(&vertexbufferDesc, &InitVertexData, &vertexBuffer),
		"failed to create vertex buffer");

	return vertexBuffer;
}

ComPtr<ID3D11Buffer> KhanDx::CreateIndexBuffer(ID3D11Device* pDevice, const void* pSysMem, UINT byteWidth) noexcept
{
	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitIndexData{};
	InitIndexData.pSysMem = pSysMem;
	InitIndexData.SysMemPitch = 0U;
	InitIndexData.SysMemSlicePitch = 0U;

	D3D11_BUFFER_DESC indexbufferDesc{};
	indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexbufferDesc.ByteWidth = byteWidth;
	indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexbufferDesc.CPUAccessFlags = 0U;
	indexbufferDesc.MiscFlags = 0U;

	ComPtr<ID3D11Buffer> indexBuffer;
	ThrowIfFailed(pDevice->CreateBuffer(&indexbufferDesc, &InitIndexData, &indexBuffer),
		"Failed to create index buffer");

	return indexBuffer;
}

ComPtr<ID3D11BlendState> KhanDx::CreateBlendState_Alpha(ID3D11Device* pDevice) noexcept
{
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	ComPtr<ID3D11BlendState> blendState;
	pDevice->CreateBlendState(&blendDesc, &blendState);
	return blendState;
}

ComPtr<ID3DBlob> KhanDx::CreateShaderBlob(std::string_view shaderName)
{
#if defined(DEBUG) || defined(_DEBUG)
	std::wstring fullPath{ L"..\\bin\\Debug-x64\\" };
#else
	std::wstring fullPath{ L"..\\bin\\Release-x64\\" };
#endif
	fullPath += std::wstring(shaderName.begin(), shaderName.end()) + L".cso";

	ComPtr<ID3DBlob> pBlob;
	HRESULT hr = ::D3DReadFileToBlob(fullPath.data(), &pBlob);
	ThrowIfFailed(hr, std::format("failed to read shader: {:s}", shaderName)/*std::string{ "failed to read shader: " } + std::string{ shaderName }*/);

	return pBlob;
}

ComPtr<ID3D11PixelShader> KhanDx::CreatePixelShader(ID3D11Device* pDevice, std::string_view shaderName)
{
	ComPtr<ID3DBlob> pBlob = CreateShaderBlob(shaderName);

	ComPtr<ID3D11PixelShader> pPixselShader;
	ThrowIfFailed(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
		nullptr, &pPixselShader),
		"failed to create pixel shader");

	return pPixselShader;
}

ComPtr<ID3D11VertexShader> KhanDx::CreateVertexShader(ID3D11Device* pDevice, ComPtr<ID3DBlob> pBlob) noexcept
{
	ComPtr<ID3D11VertexShader> pVertexShader;
	ThrowIfFailed(pDevice->CreateVertexShader(
		pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
		nullptr, &pVertexShader),
		"failed to create pixel shader");
	return pVertexShader;
}

ComPtr<ID3D11InputLayout> KhanDx::CreateInputLayout(ID3D11Device* pDevice, ComPtr<ID3DBlob> pBlob, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements) noexcept
{
	ComPtr<ID3D11InputLayout> pInputLayout;
	HRESULT hr = pDevice->CreateInputLayout(elementDescs, numElements,
		pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

	ThrowIfFailed(hr, "Failed to create input layout");

	return pInputLayout;
}

ComPtr<ID3D11ShaderResourceView> KhanDx::CreateSRV_StructBuf(ID3D11Device* pDevice, ComPtr<ID3D11Resource> pBuf, UINT firstElement, UINT numElements) noexcept
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = firstElement;
	srvDesc.Buffer.NumElements = numElements;

	ComPtr<ID3D11ShaderResourceView> pSrv;
	pDevice->CreateShaderResourceView(pBuf.Get(), &srvDesc, &pSrv);
	return pSrv;
}

ComPtr<ID3D11ShaderResourceView> KhanDx::CreateSRV_Texture2D(ID3D11Device* pDevice, std::filesystem::path filePath)
{
	int width{}, height{}, channels{};
	uint8_t* pImageData = stbi_load(filePath.string().data(), &width, &height, &channels, 0);
	if (nullptr == pImageData)
	{
		KHAN_ERROR(std::format("Failed to stbi load, filePath: {:s}", filePath.string()));
		throw std::exception("Failed to stbi load");
	}
	ComPtr<ID3D11ShaderResourceView> pSrv;
	try
	{
		pSrv = CreateSRV_Texture2D(pDevice, pImageData, width, height, channels);
	}
	catch (...)
	{
		stbi_image_free(pImageData);
		throw std::exception("failed to create shader resource view");
	}
	stbi_image_free(pImageData);

	return pSrv;
}

ComPtr<ID3D11ShaderResourceView> KhanDx::CreateSRV_Texture2D(ID3D11Device* pDevice, const aiTexture* pAiTexture)
{
	int width{}, height{}, channels{};
	uint8_t* pImageData = stbi_load_from_memory(reinterpret_cast<const uint8_t*>(pAiTexture->pcData), pAiTexture->mWidth, &width, &height, &channels, 0);
	if (nullptr == pImageData)
	{
		KHAN_ERROR("Failed to stbi load from memory");
		throw std::exception("Failed to stbi load from memory");
	}

	ComPtr<ID3D11ShaderResourceView> pSrv;
	try
	{
		pSrv = CreateSRV_Texture2D(pDevice, pImageData, width, height, channels);
	}
	catch (...)
	{
		stbi_image_free(pImageData);
		throw std::exception("failed to create shader resource view");
	}

	stbi_image_free(pImageData);

	return pSrv;
}

ComPtr<ID3D11ShaderResourceView> KhanDx::CreateSRV_Texture2D(ID3D11Device* pDevice, const uint8_t* pImageData, int width, int height, int channels)
{
	assert(channels >= 3 && "channels is less than three, is it possible?");
	
	// if channel is three, convert to four
	std::vector<uint8_t> imageRGBA;
	if (channels < 4)
	{
		imageRGBA.reserve(sizeof(uint8_t) * 4 * width * height);

		const uint8_t* First = pImageData;
		for (int i = 0; i < width * height; i++) {
			const uint8_t* Last = First + channels;
			imageRGBA.insert(imageRGBA.end(), First, Last);
			for (int j{}; j < 4 - channels; j++) { imageRGBA.push_back(255); }
			First = Last;
		}

		pImageData = imageRGBA.data();
	}

	D3D11_SUBRESOURCE_DATA InitTextureData{};
	InitTextureData.pSysMem = pImageData;
	InitTextureData.SysMemPitch = sizeof(uint8_t) * 4 * width;

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1U;
	textureDesc.ArraySize = 1U;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1U;
	textureDesc.SampleDesc.Quality = 0U;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ComPtr<ID3D11Texture2D> pTexture2D;
	HRESULT hr = pDevice->CreateTexture2D(&textureDesc, &InitTextureData, &pTexture2D);
	ThrowIfFailed(hr, "Failed to create texture");

	ComPtr<ID3D11ShaderResourceView> pSrv;
	hr = pDevice->CreateShaderResourceView(pTexture2D.Get(), nullptr, &pSrv);
	ThrowIfFailed(hr, "Failed to create SRV");

	return pSrv;
}

