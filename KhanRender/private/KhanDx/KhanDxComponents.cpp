#include "pch.h"
#include "KhanDxComponents.h"
#include "KhanDx/KhanDxUtils.h"

ComPtr<ID3D11RasterizerState> KhanDx::CreateRSState_Solid(ComPtr<ID3D11Device> d3d_device) noexcept
{
	ComPtr<ID3D11RasterizerState> rsstate;
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;

	ThrowIfFailed(d3d_device->CreateRasterizerState(&desc, &rsstate),
		"failed to create RS state");

	return rsstate;
}

ComPtr<ID3D11RasterizerState> KhanDx::CreateRSState_Solid_NoCulling(ComPtr<ID3D11Device> d3d_device) noexcept
{
	ComPtr<ID3D11RasterizerState> rsstate;
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;

	ThrowIfFailed(d3d_device->CreateRasterizerState(&desc, &rsstate),
		"failed to create RS state");

	return rsstate;
}

ComPtr<ID3D11RasterizerState> KhanDx::CreateRSState_WireFrame(ComPtr<ID3D11Device> d3d_device) noexcept
{
	ComPtr<ID3D11RasterizerState> rsstate;
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;

	ThrowIfFailed(d3d_device->CreateRasterizerState(&desc, &rsstate),
		"Failed to Create Rasterizer State");

	return rsstate;
}

ComPtr<ID3D11DepthStencilState> KhanDx::CreateDSState_Default(ComPtr<ID3D11Device> d3d_device) noexcept
{
	ComPtr<ID3D11DepthStencilState> dsstate;
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC(D3D11_DEFAULT);
	ThrowIfFailed(
		d3d_device->CreateDepthStencilState(&dsDesc, &dsstate),
		"Failed to create depth stencil state");

	return dsstate;
}

ComPtr<ID3D11Buffer> KhanDx::CreateVertexBuffer(ComPtr<ID3D11Device> d3d_device, const void* pSysMem, UINT byteWidth) noexcept
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
	ThrowIfFailed(d3d_device->CreateBuffer(&vertexbufferDesc, &InitVertexData, &vertexBuffer),
		"failed to create vertex buffer");

	return vertexBuffer;
}

ComPtr<ID3D11Buffer> KhanDx::CreateIndexBuffer(ComPtr<ID3D11Device> d3d_device, const void* pSysMem, UINT byteWidth) noexcept
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
	ThrowIfFailed(d3d_device->CreateBuffer(&indexbufferDesc, &InitIndexData, &indexBuffer),
		"Failed to create index buffer");

	return indexBuffer;
}

ComPtr<ID3D11BlendState> KhanDx::CreateBlendState_Alpha(ComPtr<ID3D11Device> d3d_device) noexcept
{
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	ComPtr<ID3D11BlendState> blendState;
	d3d_device->CreateBlendState(&blendDesc, &blendState);
	return blendState;
}

ComPtr<ID3DBlob> KhanDx::CreateShaderBlob(std::string_view cso_name)
{
#if defined(DEBUG) || defined(_DEBUG)
	std::wstring fullPath{ L"..\\bin\\Debug-x64\\" };
#else
	std::wstring fullPath{ L"..\\bin\\Release-x64\\" };
#endif
	fullPath += std::wstring(cso_name.begin(), cso_name.end());

	ComPtr<ID3DBlob> ShaderBlob;
	ThrowIfFailed(::D3DReadFileToBlob(fullPath.data(), &ShaderBlob),
		std::string{ "failed to read file: " } + std::string{ cso_name });

	return ShaderBlob;
}

ComPtr<ID3D11PixelShader> KhanDx::CreatePixelShader(ComPtr<ID3D11Device> d3d_device, std::string_view cso_name)
{
	ComPtr<ID3DBlob> pixelShader_blob = CreateShaderBlob(cso_name);

	ComPtr<ID3D11PixelShader> pixelShader;
	ThrowIfFailed(d3d_device->CreatePixelShader(
		pixelShader_blob->GetBufferPointer(), pixelShader_blob->GetBufferSize(),
		nullptr, &pixelShader),
		"failed to create pixel shader");

	return pixelShader;
}

ComPtr<ID3D11VertexShader> KhanDx::CreateVertexShader(ComPtr<ID3D11Device> d3d_device, ComPtr<ID3DBlob> pShaderBlob) noexcept
{
	ComPtr<ID3D11VertexShader> vertexShader;
	ThrowIfFailed(d3d_device->CreateVertexShader(
		pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(),
		nullptr, &vertexShader),
		"failed to create pixel shader");
	return vertexShader;
}

KhanDx::D3D11VertexShaderAndInputLayout KhanDx::CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device> d3d_device, std::string_view cso_name, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements)
{
	ComPtr<ID3DBlob> vertexShader_blob = CreateShaderBlob(cso_name);
	auto vertexShader = CreateVertexShader(d3d_device, vertexShader_blob.Get());
	auto inputLayout = CreateInputLayout(d3d_device, vertexShader_blob.Get(), elementDescs, numElements);
	return { std::move(vertexShader) , std::move(inputLayout) };
}

ComPtr<ID3D11InputLayout> KhanDx::CreateInputLayout(ComPtr<ID3D11Device> d3d_device, ComPtr<ID3DBlob> pShaderBlob, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements) noexcept
{
	ComPtr<ID3D11InputLayout> inputLayout;
	ThrowIfFailed(d3d_device->CreateInputLayout(elementDescs, numElements,
		pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &inputLayout),
		"failed to create input layout");

	return inputLayout;
}
