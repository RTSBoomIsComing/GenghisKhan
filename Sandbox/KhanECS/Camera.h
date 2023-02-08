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

namespace KhanECS::Entity
{
	using namespace entt::literals;

	entt::entity MakeCamera(entt::registry& reg, float aspectRatio) noexcept;
}

namespace KhanECS::System
{
	DirectX::XMMATRIX GetViewMatrix(entt::registry const& reg) noexcept;

	DirectX::XMMATRIX GetProjectionMatrix
	(
		float aspectRatio = 4.0F / 3,
		float fovAngleY = DirectX::XM_PI / 2,
		float nearZ = 1.0F,
		float farZ = std::numeric_limits<float>::max()) noexcept;
}



