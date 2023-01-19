#pragma once

#include "Khan.h"

#include "System/Systems.h"
#include <entt/entity/registry.hpp>
class Game : public Khan::Application
{
public:
	Game(int window_width, int window_height, std::wstring window_name)
		:
		Khan::Application(window_width, window_height, window_name) {}
	
	void GameLogic() override
	{
		// 1. use InputManager; or 2. process input on systems;
		// 1) if (inputmanager.GetKeyboard('w') {...};
		// 2) on some Systems, do 1);
		// UnitControlSystem::UnitSelect();
	}
};