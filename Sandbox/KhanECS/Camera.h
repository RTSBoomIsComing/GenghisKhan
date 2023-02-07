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

		// below annotated code allow converting XMVector to Position, but it is too confusing so that I disabled it
		//Position() noexcept = default;
		//Position(float x, float y, float z) noexcept : x(x), y(y), z(z) {}
		//Position(DirectX::XMVECTOR const& other) noexcept
		//{
		//	DirectX::XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), other);
		//}
		//Position& operator=(DirectX::XMVECTOR const& other) noexcept
		//{
		//	DirectX::XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(this), other);
		//	return *this;
		//}

		operator XMFLOAT3() const { return { x, y, z }; }
		operator XMFLOAT3& () { return reinterpret_cast<XMFLOAT3&>(*this); }
		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(this));
		}
	};
	struct Rotation
	{
		float x{}, y{}, z{};

		operator XMFLOAT3() const { return { x, y, z }; }
		operator XMFLOAT3& () { return reinterpret_cast<XMFLOAT3&>(x); }
		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(this));
		}
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



