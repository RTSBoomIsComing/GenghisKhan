#pragma once

#include "KhanEcs/Components.h"

// additional dependencies
#include <DirectXMath.h>
#include <entt/fwd.hpp>
#include <limits>

namespace KhanECS::Entity
{
	using namespace entt::literals;

	entt::entity MakeCamera(entt::registry& reg) noexcept;
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



