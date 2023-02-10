#pragma once

#include "KhanECS/Components.h"

// additional dependencies
#include <DirectXMath.h>
#include <entt/entt.hpp>

namespace KhanECS::System
{
	template<typename T>
	std::vector<DirectX::XMMATRIX> GetWorldMatrices(entt::registry const& reg) noexcept
	{
		using namespace DirectX;
		using namespace KhanECS::Component;
		using namespace entt::literals;

		auto view = reg.view<T, Position, Rotation>();
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