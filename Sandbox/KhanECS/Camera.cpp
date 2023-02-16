#include "pch.h"
#include "Camera.h"



entt::entity KhanECS::Entity::MakeCamera(entt::registry& reg) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;

	entt::entity e = reg.create();
	reg.emplace<Camera>(e);
	reg.emplace<Position>(e, 0.0F, 300.0F, 0.0F);
	reg.emplace<Rotation>(e, 0.0F, 0.0F, 0.0F);

	return e;
}

// get view matrix of first Camera
DirectX::XMMATRIX KhanECS::System::GetViewMatrix(entt::registry const& reg) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;
	auto view = reg.view<Camera, Position, Rotation>();

	for (entt::entity e : view)
	{
		Position const& pos = view.get<Position>(e);
		Rotation const& rot = view.get<Rotation>(e);

		XMMATRIX viewMat = XMMatrixInverse(nullptr,
			XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rot))
			* XMMatrixTranslationFromVector(XMLoadFloat3(&pos)));
		return viewMat;
	}

	return XMMatrixIdentity();
}

DirectX::XMMATRIX KhanECS::System::GetProjectionMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ) noexcept
{
	using namespace DirectX;
	XMMATRIX projMat = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
	return projMat;
}
