#pragma once
#include <d3d11.h>
#include <string_view>
#include "DxCore.h"
#include "DxUtility.h"
namespace Khan
{
	ComPtr<ID3D11Buffer> CreateVertexBuffer(const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3D11Buffer> CreateIndexBuffer(const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3DBlob> CreateShaderBlob(std::string_view fileName);
	ComPtr<ID3D11PixelShader> CreatePixelShader(std::string_view fileName);
	//void CreateDSView(int width, int height, ID3D11DepthStencilView** ppDsview);

	template<typename T, UINT count>
	ComPtr<ID3D11Buffer> CreateDynamicCBuffer() noexcept
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

		Khan::ThrowIfFailed(
			dx_device->CreateBuffer(&desc, &s_data, &cbuffer),
			"failed to create constant buffer");

		return cbuffer;
	}
}