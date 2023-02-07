#pragma once


#include "KhanECS/Camera.h"

// additional dependencies
#include <DirectXMath.h>
#include <entt/fwd.hpp>

namespace KhanECS::System
{

	[[nodiscard]]
	DirectX::XMMATRIX MouseEdgeScroll(entt::registry& reg, DirectX::XMFLOAT2 velocity) noexcept;
}