#include "pch.h"
#include "Cube.h"
#include "KhanECS/Components.h"

entt::entity KhanECS::Entity::MakeCube(entt::registry& reg, DirectX::XMFLOAT3 position) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;

	entt::entity e = reg.create();
	reg.emplace<Cube>(e);
	reg.emplace<Position>(e, position);
	reg.emplace<Rotation>(e);

	return e;
}