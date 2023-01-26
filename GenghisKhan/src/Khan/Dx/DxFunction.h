#pragma once
#include <d3d11.h>
#include <string_view>
namespace Khan
{
	ComPtr<ID3D11Buffer> CreateVertexBuffer(const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3D11Buffer> CreateIndexBuffer(const void* pSysMem, UINT byteWidth) noexcept;
	ComPtr<ID3D11PixelShader> CreatePixelShader(std::string_view fileName);
	//void CreateDSView(int width, int height, ID3D11DepthStencilView** ppDsview);
}
