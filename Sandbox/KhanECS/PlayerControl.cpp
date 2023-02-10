#include "pch.h"
#include "PlayerControl.h"
#include "KhanECS/Components.h"

// later PlayerController component will succeed this function, for the time being, use it
void KhanECS::System::MouseEdgeScroll(entt::registry& reg, DirectX::XMFLOAT2 velocity) noexcept
{	
	using namespace DirectX;
	using namespace entt::literals;
	using namespace KhanECS::Component;
	auto view = reg.view<Camera, Position, Rotation>();

	for (entt::entity const e : view)
	{
		XMFLOAT3& pos = view.get<Position>(e);
		XMVECTOR newPos = XMLoadFloat3(&pos) + 0.1F * XMLoadFloat2(&velocity);
		XMStoreFloat3(&pos, newPos);
	}
}
