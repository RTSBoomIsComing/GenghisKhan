#pragma once

// additional dependencies
#include <DirectXMath.h>
#include <entt/fwd.hpp>
#include <limits>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4X4;

namespace KhanECS::Component
{
	struct Position
	{
		Position(float x, float y, float z) noexcept : pos(x, y, z) {}
		XMFLOAT3 pos{ 0.0F, 0.0F, 0.0F };
	};
	struct Rotation
	{
		XMFLOAT3 rot{ 0.0F, 0.0F, 0.0F };
	};
	struct ForwardVector
	{
		XMFLOAT3 vec{ 0.0F, 0.0F, 1.0F };
	};
	struct UpVector
	{
		XMFLOAT3 vec{ 0.0F, 1.0F, 0.0F };
	};

	struct Perspective
	{
		float fovAngleY{ DirectX::XM_PI / 4 };
		float aspectRatio{ static_cast<float>(4) / 3 };
		float nearZ{ 1.0F };
		float farZ{ std::numeric_limits<float>::max() };
	};

	struct ViewProjMatrix
	{
		XMFLOAT4X4 mat{};
	};

}

namespace KhanECS::Entity
{
	using namespace entt::literals;
	
	entt::entity MakeCamera(entt::registry& reg, float aspectRatio) noexcept;
}

namespace KhanECS::System
{
	void MoveCamera(entt::registry& reg, XMFLOAT2 xy) noexcept;
}



