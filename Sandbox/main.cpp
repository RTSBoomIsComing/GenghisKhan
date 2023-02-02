#pragma once
#include "pch.h"
//#include <Khan.h>
#include <format>
#include "Game2.h"

#include <KhanTools/Log.h>

int main(int, char* [])
{
	KHAN_INFO("Run Genghis Khan");
	

	//int result{};
	//std::unique_ptr<Khan::Application> game = std::make_unique<Game>() ;
	//result = game->Run();
	//return result;
	Game2 game;

	MSG msg{};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			game.Run();
		}
	}

	return static_cast<int>(msg.wParam);
}