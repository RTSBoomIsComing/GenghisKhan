#pragma once

// additional dependencies
#include <DirectXMath.h>
#include <entt/fwd.hpp>
#include <limits>
#include <cmath>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4X4;

namespace KhanECS::Component
{
	struct Position
	{
		float x{}, y{}, z{};
		constexpr operator XMFLOAT3() const { return { x, y, z }; }
		operator XMFLOAT3& () { return reinterpret_cast<XMFLOAT3&>(x); }
	};
	struct Rotation
	{
		float x{}, y{}, z{};
		constexpr operator XMFLOAT3() const { return { x, y, z }; }
		operator XMFLOAT3& () { return reinterpret_cast<XMFLOAT3&>(x); }
	};
}

namespace KhanECS::Entity
{
	using namespace entt::literals;

	entt::entity MakeCamera(entt::registry& reg, float aspectRatio) noexcept;
}

namespace KhanECS::System
{
	DirectX::XMMATRIX GetProjectionMatrix
	(
		float aspectRatio = 4.0F / 3,
		float fovAngleY = DirectX::XM_PI / 2,
		float nearZ = 1.0F,
		float farZ = std::numeric_limits<float>::max()) noexcept;
}



