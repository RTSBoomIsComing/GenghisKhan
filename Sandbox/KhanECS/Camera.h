#pragma once
#include "KhanEcs/Components.h"

// additional dependencies
#include <DirectXMath.h>
#include <entt/fwd.hpp>
#include <limits>


namespace KhanECS::Component
{
	struct Camera : EntityName {};
	struct CameraProjectionInfo
	{
		float aspectRatio = 4.0F / 3;
		float fovAngleY = DirectX::XM_PI / 2;
		float nearZ = 1.0F;
		float farZ = 10000.0F;
	};
}

namespace KhanECS::Entity
{
	entt::entity MakeCamera(entt::registry& reg) noexcept;
}

namespace KhanECS::System
{
	DirectX::XMMATRIX GetViewProjectionMatrix(entt::registry const& reg) noexcept;
}



