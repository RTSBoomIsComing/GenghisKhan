#include "pch.h"
#include "Camera.h"



entt::entity KhanECS::Entity::MakeCamera(entt::registry& reg, float aspectRatio) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;

	entt::entity e = reg.create();
	reg.emplace<entt::tag<"Camera"_hs>>(e);
	reg.emplace<Position>(e, 0.0F, 0.0F, -10.0F);
	reg.emplace<Rotation>(e);

	return e;
}

// get view matrix of first Camera
DirectX::XMMATRIX KhanECS::System::GetViewMatrix(entt::registry const& reg) noexcept
{
	using namespace DirectX;
	using namespace entt::literals;
	using namespace KhanECS::Component;
	auto view  = reg.view<entt::tag<"Camera"_hs>, Position, Rotation>();

	for (entt::entity e : view)
	{
		//XMFLOAT3& rot = view.get<Rotation>(e).vec;

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
