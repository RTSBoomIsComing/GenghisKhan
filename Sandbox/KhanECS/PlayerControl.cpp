#include "pch.h"
#include "PlayerControl.h"

void KhanECS::System::MouseEdgeScroll(entt::registry& reg, DirectX::XMFLOAT2 velocity) noexcept
{
	using namespace DirectX;
	using namespace entt::literals;
	using namespace KhanECS::Entity;
	using namespace KhanECS::Component;
	auto view = reg.view<entt::tag<"Camera"_hs>, Position, ForwardVector, UpVector, Perspective, ViewProjMatrix>();

	for (entt::entity const e : view)
	{
		XMFLOAT3& pos = view.get<Position>(e).pos;
		XMVECTOR newPos = XMLoadFloat3(&pos) + XMLoadFloat2(&velocity);
		XMStoreFloat3(&pos, newPos);
		
		XMFLOAT3 const& eyeDir = view.get<ForwardVector>(e).vec;
		XMFLOAT3 const& up = view.get<UpVector>(e).vec;

		auto const& perspective = view.get<Perspective>(e);
		float const fov = perspective.fovAngleY;
		float const aspectRatio = perspective.aspectRatio;
		float const nearZ = perspective.nearZ;
		float const farZ = perspective.farZ;

		auto& viewProjMatrix = view.get<ViewProjMatrix>(e).mat;

		auto newViewProjMatrix = XMMatrixLookToLH(newPos, XMLoadFloat3(&eyeDir), XMLoadFloat3(&up))
			* XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);

		XMStoreFloat4x4(&viewProjMatrix, newViewProjMatrix);
	}
}
