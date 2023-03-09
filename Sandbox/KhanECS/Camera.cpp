#include "pch.h"
#include "Camera.h"



entt::entity KhanECS::Entity::MakeCamera(entt::registry& reg) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;

	entt::entity e = reg.create();
	reg.emplace<Camera>(e);
	reg.emplace<CameraProjectionInfo>(e);
	reg.emplace<Position>(e, 0.0F, 300.0F, 0.0F);
	reg.emplace<Rotation>(e, 0.0F, 0.0F, 0.0F);
	return e;
}

// get view matrix of first Camera
DirectX::XMMATRIX KhanECS::System::GetViewProjectionMatrix(entt::registry const& reg) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;
	auto view = reg.view<Camera, CameraProjectionInfo, Position, Rotation>();

	for (entt::entity e : view)
	{
		const Position& pos = view.get<Position>(e);
		const Rotation& rot = view.get<Rotation>(e);
		const auto& projInfo = view.get<CameraProjectionInfo>(e);

		XMMATRIX viewMat = XMMatrixInverse(nullptr,
			XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rot))
			* XMMatrixTranslationFromVector(XMLoadFloat3(&pos)));
		
		return viewMat * projInfo.ProjectionMatrix;
	}

	return XMMatrixIdentity();
}
