#pragma once

#include "KhanECS/Components.h"

// additional dependencies
#include <DirectXMath.h>
#include <entt/fwd.hpp>

namespace KhanECS::System
{
	template<typename ...Args>
	std::vector<DirectX::XMMATRIX> GetWorldMatrices(entt::registry const& reg) noexcept
	{
		using namespace DirectX;
		using namespace KhanECS::Component;

		auto view = reg.view<Args..., Position, Rotation>();
		std::vector<DirectX::XMMATRIX> matrices;
		for (entt::entity e : view)
		{
			Position pos = view.get<Position>(e);
			Rotation rot = view.get<Rotation>(e);

			auto mat = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rot)) * XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
			matrices.push_back(mat);
		}

		return matrices;
	}

}