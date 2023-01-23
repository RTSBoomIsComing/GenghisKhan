#pragma once

#include "Khan/Application.h"
#include "System/Systems.h"
#include <entt/entity/registry.hpp>
class Game : public Khan::Application
{
public:
	Game() noexcept;
	~Game() noexcept = default;
private:
	entt::registry reg;
public:
	void Logic() override;
	void Render() override;
	void ImGuiRender_Impl() override;
};




