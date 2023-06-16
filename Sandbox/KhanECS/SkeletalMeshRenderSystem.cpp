#include "pch.h"
#include "SkeletalMeshRenderSystem.h"
#include "Camera.h"
//#include "Systems.h"
#include "Character.h"

#include <KhanRender/SkeletalMeshRenderer.h>

#include <unordered_set>


// standard libraries

KhanECS::System::SkeletalMeshRenderSystem::SkeletalMeshRenderSystem()
{
	//m_KnightRenderer = std::make_unique<KhanRender::SkeletalMeshRenderer>(m_mainRenderer, "D:\\Assets\\Mixamo\\Knight D Pelegrini.fbx");
	//m_PaladinRenderer = std::make_unique<KhanRender::SkeletalMeshRenderer>(m_mainRenderer, "D:\\Assets\\Mixamo\\Paladin J Nordstrom.fbx");
}

void KhanECS::System::SkeletalMeshRenderSystem::Update(float deltaTime, entt::registry& reg)
{
	using namespace DirectX;
	using namespace KhanECS::Component;

	static float accTime{}; // For test
	accTime += deltaTime;


	auto view = reg.view<SkeletalMeshComponent, Position, Rotation>();
	for (entt::entity e : view)
	{
		const auto& renderer = view.get<SkeletalMeshComponent>(e).pRenderer;
		const Position& pos = view.get<Position>(e);
		const Rotation& rot = view.get<Rotation>(e);

		auto mat = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rot)) * XMMatrixTranslationFromVector(XMLoadFloat3(&pos));

		renderer->AddInstance(mat, 0, accTime);
	}
}
