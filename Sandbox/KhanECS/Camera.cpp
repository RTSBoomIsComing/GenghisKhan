#include "pch.h"
#include "Camera.h"



entt::entity KhanECS::Entity::MakeCamera(entt::registry& reg, float aspectRatio) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;

	const entt::entity e = reg.create();
	reg.emplace<entt::tag<"Camera"_hs>>(e);

	const Position pos{ 0.0F, 0.0F, -10.0F };
	reg.emplace<Position>(e, pos);
	reg.emplace<ForwardVector>(e);
	reg.emplace<UpVector>(e);
	reg.emplace<Perspective>(e, aspectRatio);

	constexpr ForwardVector fwd{};
	constexpr UpVector up{};
	constexpr Perspective perspective{};
	ViewProjMatrix viewProjMat{};
	XMStoreFloat4x4(&viewProjMat.mat,
		XMMatrixLookToLH(XMLoadFloat3(&pos.pos), XMLoadFloat3(&fwd.vec), XMLoadFloat3(&up.vec))
		* XMMatrixPerspectiveFovLH(perspective.fovAngleY, perspective.aspectRatio, perspective.nearZ, perspective.farZ));

	reg.emplace<ViewProjMatrix>(e);
	return e;
}

void KhanECS::System::MoveCamera(entt::registry& reg, XMFLOAT2 xy) noexcept
{
	using namespace KhanECS::Component;
	using namespace entt::literals;

	auto view = reg.view<entt::tag<"Camera"_hs>, Position, Rotation>();
	for (auto e : view)
	{
		auto& pos = view.get<Position>(e).pos;
		pos.x += xy.x;
		pos.y += xy.y;
		//... todo ... or is it really needed?
	}
}


