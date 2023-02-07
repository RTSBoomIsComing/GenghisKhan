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

DirectX::XMMATRIX KhanECS::System::GetProjectionMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ) noexcept
{
	using namespace DirectX;
	XMMATRIX projMat = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
	return projMat;
}
