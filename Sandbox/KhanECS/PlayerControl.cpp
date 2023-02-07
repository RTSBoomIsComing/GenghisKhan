#include "pch.h"
#include "PlayerControl.h"


// later PlayerController component will succeed this function, for the time being, use it
DirectX::XMMATRIX KhanECS::System::MouseEdgeScroll(entt::registry& reg, DirectX::XMFLOAT2 velocity) noexcept
{	
	using namespace DirectX;
	using namespace entt::literals;
	using namespace KhanECS::Entity;
	using namespace KhanECS::Component;
	auto view = reg.view<entt::tag<"Camera"_hs>, Position, Rotation>();

	for (entt::entity const e : view)
	{
		//XMFLOAT3& rot = view.get<Rotation>(e).vec;

		// store new position to Position::vec
		XMFLOAT3& pos = view.get<Position>(e);
		XMVECTOR const newPos = XMLoadFloat3(&pos) + XMLoadFloat2(&velocity);
		XMStoreFloat3(&pos, newPos);

		// store new translation matrix to Position::mat
		//XMFLOAT4X4& viewMatrix = view.get<Position>(e).mat;
		XMMATRIX const newViewMatrix = XMMatrixTranslationFromVector(newPos);
		//XMStoreFloat4x4(&viewMatrix, newViewMatrix);
		
		// later have to extract GetViewMatrix function from this function
		return XMMatrixInverse(nullptr, newViewMatrix);
	}
	return XMMatrixIdentity();
}
