#pragma once
#include <KhanECS/Components.h>

#include <entt/entity/fwd.hpp>
#include <DirectXMath.h>

namespace KhanECS::Component
{
	struct Character : EntityName {};
	struct Archer {};
	struct Paladin {};
	struct Knight {};
}

namespace KhanECS::Entity
{
	entt::entity MakeCharacter(entt::registry& reg, DirectX::XMFLOAT3 position) noexcept;
}

namespace KhanECS::System
{

}
