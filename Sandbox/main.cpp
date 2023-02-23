#pragma once
#include "pch.h"
#include "Game2.h"
#include <KhanTools/Log.h>
#include <KhanAnim/SkeletalAnimation.h>

int main(int, char* [])
{
	KHAN_INFO("Run Genghis Khan");

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