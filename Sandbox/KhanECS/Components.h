#pragma once
#include <DirectXMath.h>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;


namespace KhanECS::Component
{
	struct Position : XMFLOAT3
	{
		constexpr Position() noexcept = default;
		constexpr Position(float x, float y, float z) noexcept : XMFLOAT3(x, y, z) {}
	};
	struct Rotation : XMFLOAT3
	{
		constexpr Rotation() noexcept = default;
		constexpr Rotation(float x, float y, float z) noexcept : XMFLOAT3(x, y, z) {}
	};
}