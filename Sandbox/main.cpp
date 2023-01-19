#pragma once
#include "pch.h"

#include <Khan.h>
// #include <Khan/Application.h>

#include <Khan/Window/Window.h>
#include "Game.h"
#include "DebugGui.h"


#include <memory>
int main(int, char*[])
{
	KHAN_INFO("Run Genghis Khan");
	int result{};
	try
	{
		auto window = std::make_unique<Khan::Window>(1280, 960, L"Genghis Khan(Sand box)");
		std::unique_ptr<Khan::IGame> game = std::make_unique<Game>();
		std::unique_ptr<Khan::IImGui> gui = std::make_unique<DebugGui>();
		Khan::Application app(window, game, gui);
		result = app.Run();
	}
	catch (std::exception& e)
	{
		KHAN_ERROR(e.what());
		return 1;
	}
	return result;
}
