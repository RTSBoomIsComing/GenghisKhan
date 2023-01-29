#include "pch.h"
#include "DxFunction.h"
#include "Dx/DxUtility.h"
#include "DxCore.h"
namespace Khan
{
	ComPtr<ID3D11Buffer> CreateVertexBuffer(const void* pSysMem, UINT byteWidth) noexcept
	{
		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitVertexData{};
		InitVertexData.pSysMem = pSysMem;
		InitVertexData.SysMemPitch = 0u; // not used 
		InitVertexData.SysMemSlicePitch = 0u; // not used

		// Fill in a buffer description.
		D3D11_BUFFER_DESC vertexbufferDesc{};
		vertexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexbufferDesc.ByteWidth = byteWidth;
		vertexbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexbufferDesc.CPUAccessFlags = 0u;
		vertexbufferDesc.MiscFlags = 0u;

		ComPtr<ID3D11Buffer> vertexBuffer;
		Khan::ThrowIfFailed(dx_device->CreateBuffer(&vertexbufferDesc, &InitVertexData, &vertexBuffer),
			"failed to create vertex buffer");

		return vertexBuffer;
	}
	ComPtr<ID3D11Buffer> CreateIndexBuffer(const void* pSysMem, UINT byteWidth) noexcept
	{
		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitIndexData{};
		InitIndexData.pSysMem = pSysMem;
		InitIndexData.SysMemPitch = 0u;
		InitIndexData.SysMemSlicePitch = 0u;

		D3D11_BUFFER_DESC indexbufferDesc{};
		indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexbufferDesc.ByteWidth = byteWidth;
		indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexbufferDesc.CPUAccessFlags = 0u;
		indexbufferDesc.MiscFlags = 0u;

		ComPtr<ID3D11Buffer> indexBuffer;
		Khan::ThrowIfFailed(dx_device->CreateBuffer(&indexbufferDesc, &InitIndexData, &indexBuffer),
			"failed to create index buffer");
		return indexBuffer;
	}
	ComPtr<ID3DBlob> CreateShaderBlob(std::string_view fileName)
	{
#if defined(DEBUG) || defined(_DEBUG)
		std::wstring fullFileName{ L"..\\bin\\Debug-x64\\Sandbox\\"};
#else
		std::wstring fullFileName{ L"..\\bin\\Release-x64\\Sandbox\\" };
#endif
		fullFileName += std::wstring(fileName.begin(), fileName.end());

		ComPtr<ID3DBlob> ShaderBlob; 
		Khan::ThrowIfFailed(
			::D3DReadFileToBlob(fullFileName.data(), &ShaderBlob),
			std::string{ "failed to read file: " } + std::string{ fileName });

		return ShaderBlob;
	}
	ComPtr<ID3D11PixelShader> CreatePixelShader(std::string_view fileName)
    {
		ComPtr<ID3DBlob> pixelShader_blob = CreateShaderBlob(fileName);

		ComPtr<ID3D11PixelShader> pixelShader;
		Khan::ThrowIfFailed(dx_device->CreatePixelShader(
			pixelShader_blob->GetBufferPointer(), pixelShader_blob->GetBufferSize(),
			nullptr, &pixelShader),
			"failed to create pixel shader");
        return pixelShader;
    }
	ComPtr<ID3D11VertexShader> CreateVertexShader(ID3DBlob* pShaderBlob) noexcept
	{
		ComPtr<ID3D11VertexShader> vertexShader;
		Khan::ThrowIfFailed(dx_device->CreateVertexShader(
			pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(),
			nullptr, &vertexShader),
			"failed to create pixel shader");
		return vertexShader;
	}
	ComPtr<ID3D11InputLayout> CreateInputLayout(ID3DBlob* pShaderBlob, const D3D11_INPUT_ELEMENT_DESC* elementDescs, UINT numElements) noexcept
	{
		ComPtr<ID3D11InputLayout> inputLayout;
		Khan::ThrowIfFailed(
			dx_device->CreateInputLayout(
				elementDescs, numElements,
				pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &inputLayout),
			"failed to create input layout");

		return inputLayout;
	}
	ComPtr<ID3D11BlendState> CreateBlendState_Alpha_Static() noexcept
	{
		static ComPtr<ID3D11BlendState> blendState;
		if (blendState) return blendState;

		D3D11_BLEND_DESC blendDesc{};
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		dx_device->CreateBlendState(&blendDesc, &blendState);
		return blendState;
	}
	//void CreateDSView(int width, int height, ID3D11DepthStencilView** ppDsview)
	//{
	//	// create depth stencil state
	//	ComPtr<ID3D11DepthStencilState> ds_state;
	//	D3D11_DEPTH_STENCIL_DESC ds_desc{};
	//	{
	//		ds_desc.DepthEnable = true;
	//		ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//		ds_desc.DepthFunc = D3D11_COMPARISON_LESS;
	//	}

	//	Khan::ThrowIfFailed(
	//		dx_device->CreateDepthStencilState(&ds_desc, &ds_state),
	//		"failed to create depth stencil state");

	//	dx_context->OMSetDepthStencilState(ds_state.Get(), 1u);

	//	ComPtr<ID3D11Texture2D> ds_texture;
	//	D3D11_TEXTURE2D_DESC texture_desc{};
	//	{
	//		texture_desc.Width = width;
	//		texture_desc.Height = height;
	//		texture_desc.MipLevels = 1u;
	//		texture_desc.ArraySize = 1u;
	//		texture_desc.Format = DXGI_FORMAT_D32_FLOAT; //about D32, D := depth, special format for depth
	//		texture_desc.SampleDesc.Count = 1u;
	//		texture_desc.SampleDesc.Quality = 0u;
	//		texture_desc.Usage = D3D11_USAGE_DEFAULT;
	//		texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//	}
	//	Khan::ThrowIfFailed(
	//		dx_device->CreateTexture2D(&texture_desc, nullptr, &ds_texture),
	//		"failed to create texture of depth stencil");

	//	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc{};
	//	{
	//		dsv_desc.Format = DXGI_FORMAT_D32_FLOAT;
	//		dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//		dsv_desc.Texture2D.MipSlice = 0u;
	//	}
	//	Khan::ThrowIfFailed(
	//		dx_device->CreateDepthStencilView(ds_texture.Get(), &dsv_desc, ppDsview),
	//		"failed to create depth stencil view");
	//}

}
