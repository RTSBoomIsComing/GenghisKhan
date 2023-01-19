#pragma once

#include <Khan/IGame.h>

#include "System/Systems.h"
#include <entt/entity/registry.hpp>
class Game : public Khan::IGame
{
public:
	virtual void Logic() override
	{
		// 1. use InputManager; or 2. process input on systems;
		// 1) if (inputmanager.GetKeyboard('w') {...};
		// 2) on some Systems, do 1);
		UnitControlSystem::UnitSelect();
	}
};