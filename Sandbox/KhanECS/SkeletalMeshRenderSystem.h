#pragma once

// standard libraries
#include <filesystem>
#include <unordered_map>
#include <memory>
// additional dependencies
#include <entt/fwd.hpp>

namespace KhanRender
{
	class Renderer;
	class SkeletalMeshRenderer;
}

namespace KhanECS::System
{
	class SkeletalMeshRenderSystem
	{
	public:
		SkeletalMeshRenderSystem();
		void Update(float deltaTime, entt::registry& reg);
	};
}

namespace KhanECS::Component
{
	struct SkeletalMeshComponent
	{
		const std::shared_ptr<KhanRender::SkeletalMeshRenderer> pRenderer{};
	};
}
