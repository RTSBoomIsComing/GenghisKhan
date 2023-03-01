#pragma once

// standard libraries
#include <filesystem>
#include <array>
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
		enum class RendererId
		{
			Archer, Max
		};
	public:
		SkeletalMeshRenderSystem(KhanRender::Renderer const& renderer);
		void Update(float deltaTime, entt::registry& reg);
		void Render();
	private:
		std::array<std::unique_ptr<KhanRender::SkeletalMeshRenderer>, static_cast<size_t>(RendererId::Max)> m_RendererList;
	};
}

namespace KhanECS::Component
{
	struct SkeletalMeshComponent
	{
		KhanECS::System::SkeletalMeshRenderSystem::RendererId rendererId{};
	};
}
