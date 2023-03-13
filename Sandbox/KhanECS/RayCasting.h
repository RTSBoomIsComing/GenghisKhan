#pragma once
#include <KhanECS/Components.h>

#include <entt/entity/fwd.hpp>
#include <DirectXMath.h>

namespace KhanECS::Component
{
	struct RayComponent
	{
		DirectX::XMFLOAT3 Start;
		DirectX::XMFLOAT3 End;
	};
}

namespace KhanECS::Entity
{
	entt::entity MakeRayFromScreen();

}