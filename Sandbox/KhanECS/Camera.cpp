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
	//reg.emplace<ForwardVector>(e);
	//reg.emplace<UpVector>(e);

	//constexpr ForwardVector fwd{};
	//constexpr UpVector up{};

	// I want to make it compile-time thing. On the other hand, make it constexpr.
	// So maybe I need custom math library... or set xmfloat4x4 directly
	//ViewProjMatrix viewProjMat{};
	//XMStoreFloat4x4(&viewProjMat.mat,
	//	XMMatrixLookToLH(XMLoadFloat3(&pos.pos), XMLoadFloat3(&fwd.vec), XMLoadFloat3(&up.vec))
	//	* XMMatrixPerspectiveFovLH(perspective.fovAngleY, perspective.aspectRatio, perspective.nearZ, perspective.farZ));

	return e;
}

DirectX::XMMATRIX KhanECS::System::GetProjectionMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ) noexcept
{
	auto projMat = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
	return projMat;
}
