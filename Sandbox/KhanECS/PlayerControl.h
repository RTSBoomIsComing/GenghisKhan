#pragma once
#include "KhanECS/Camera.h"

// additional dependencies
#include <DirectXMath.h>
#include <entt/fwd.hpp>

namespace KhanECS::System
{
	void MouseEdgeScroll(entt::registry& reg, DirectX::XMFLOAT3 velocity) noexcept;
	void SetCameraRotation(entt::registry& reg, DirectX::XMFLOAT3 const& newRot) noexcept;
}