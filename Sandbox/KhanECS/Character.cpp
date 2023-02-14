#include "pch.h"
#include "Character.h"

entt::entity KhanECS::Entity::MakeCharacter(entt::registry& reg, DirectX::XMFLOAT3 position) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;

	entt::entity e = reg.create();
	reg.emplace<Character>(e);
	reg.emplace<Position>(e, position);
	reg.emplace<Rotation>(e);

	return e;
}