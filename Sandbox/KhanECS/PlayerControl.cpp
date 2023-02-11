#include "pch.h"
#include "PlayerControl.h"
#include "KhanECS/Components.h"

// later PlayerController component will succeed this function, for the time being, use it
void KhanECS::System::MouseEdgeScroll(entt::registry& reg, DirectX::XMFLOAT3 velocity) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;
	auto view = reg.view<Camera, Position, Rotation>();

	for (entt::entity const e : view)
	{
		XMFLOAT3& rot = view.get<Rotation>(e);
		XMVECTOR forwardVector{};
		XMVECTOR rightVector{};
		XMMATRIX tramsform = XMMatrixRotationY(rot.y);
		forwardVector = XMVector3Transform({ 0.0F, 0.0F, 1.0F }, tramsform);
		rightVector = XMVector3Transform({ 1.0F, 0.0F, 0.0F }, tramsform);
		XMFLOAT3& pos = view.get<Position>(e);
		XMVECTOR newPos = XMLoadFloat3(&pos) + forwardVector * velocity.y * 10.0F + rightVector * velocity.x * 10.0F;
		XMStoreFloat3(&pos, newPos);
	}
}

void KhanECS::System::SetCameraRotation(entt::registry& reg, DirectX::XMFLOAT3 const& newRot) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;
	auto view = reg.view<Camera, Rotation>();

	for (entt::entity const e : view)
	{
		XMFLOAT3& rot = view.get<Rotation>(e);
		XMStoreFloat3(&rot, XMLoadFloat3(&newRot));
	}
}
