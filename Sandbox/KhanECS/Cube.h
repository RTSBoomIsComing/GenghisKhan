#pragma once
#include <KhanECS/Components.h>

#include <entt/entity/fwd.hpp>
#include <DirectXMath.h>


namespace KhanECS::Component
{
	struct Cube : EntityName {};
}

namespace KhanECS::Entity
{
	entt::entity MakeCube(entt::registry& reg, DirectX::XMFLOAT3 position) noexcept;
}
