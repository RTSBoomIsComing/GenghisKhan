#include "pch.h"
#include "Cube.h"
#include "KhanECS/Components.h"

entt::entity KhanECS::Entity::MakeCube(entt::registry& reg, DirectX::XMFLOAT3 position) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;
	using namespace entt::literals;

	entt::entity e = reg.create();
	reg.emplace<entt::tag<"Cube"_hs>>(e);
	reg.emplace<Position>(e, position);
	reg.emplace<Rotation>(e);

	return e;
}

std::vector<DirectX::XMMATRIX> KhanECS::System::GetCubeWorldMatrices(entt::registry const& reg) noexcept
{
	using namespace DirectX;
	using namespace KhanECS::Component;
	using namespace entt::literals;
	
	auto view = reg.view<entt::tag<"Cube"_hs>, Position, Rotation>();
	std::vector<DirectX::XMMATRIX> matrices;
	for (entt::entity e : view)
	{
		Position pos = view.get<Position>(e);
		Rotation rot = view.get<Rotation>(e);

		auto mat = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rot))* XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
		matrices.push_back(mat);
	}


	return std::move(matrices);
}
