#pragma once
#include <vector>
#include <DxCore.h>

namespace Khan
{
	template<typename T>
	class DynamicCBuffer
	{
	public:
		DynamicCBuffer() noexcept;
		ComPtr<ID3D11Buffer> D3dBuffer;
	};

	template<typename T>
	DynamicCBuffer<T>::DynamicCBuffer() noexcept
	{
		D3D11_BUFFER_DESC desc{};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0u;
		desc.ByteWidth = sizeof(T);

		T SysMem{};

		D3D11_SUBRESOURCE_DATA s_data{};
		s_data.pSysMem = &SysMem;

		Khan::ThrowIfFailed(
			dx_device->CreateBuffer(&desc, &s_data, &D3dBuffer),
			"failed to create constant buffer");
	}
}

