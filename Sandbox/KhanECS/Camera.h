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
		// set matrix as identity matrix
		constexpr Position() noexcept { mat._11 = 1.0F; mat._22 = 1.0F; mat._33 = 1.0F; mat._44 = 1.0F; }
		constexpr Position(float x, float y, float z) noexcept 
			: vec(x, y, z) 
		{
			mat._11 = 1.0F; mat._22 = 1.0F; mat._33 = 1.0F; mat._44 = 1.0F; // primary diagonal
			mat._41 = x; mat._42 = y; mat._43 = z; // for compile-time calculation, not use directxmath functions
		}
		XMFLOAT4X4 mat{};
		XMFLOAT3 vec{};
	};
	struct Rotation
	{
		// set matrix as identity matrix
		constexpr Rotation() noexcept { mat._11 = 1.0F; mat._22 = 1.0F; mat._33 = 1.0F; mat._44 = 1.0F; }
		Rotation(float x, float y, float z) noexcept
			: vec(x, y, z)
		{
			// is there a way to calculate trigonometric at compile-time?
			DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&vec)));
		}
		XMFLOAT4X4 mat{};
		XMFLOAT3 vec{};
	};
	//struct ForwardVector
	//{
	//	XMFLOAT3 vec{ 0.0F, 0.0F, 1.0F };
	//};
	//struct UpVector
	//{
	//	XMFLOAT3 vec{ 0.0F, 1.0F, 0.0F };
	//};
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



