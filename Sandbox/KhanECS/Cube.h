#pragma once
#include <entt/entity/fwd.hpp>
#include <DirectXMath.h>
#include <vector>


namespace KhanECS::Entity
{
	entt::entity MakeCube(entt::registry& reg, DirectX::XMFLOAT3 position) noexcept;
}

namespace KhanECS::System
{
	std::vector<DirectX::XMMATRIX> GetCubeWorldMatrices(entt::registry const& reg) noexcept;
}

