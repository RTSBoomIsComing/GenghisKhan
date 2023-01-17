#pragma once
#include "pch.h"

#include <WinUser.h>
#include <Khan.h>

int main(int, char**)
{
	KHAN_INFO("Run Genghis Khan");
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
			// game.Update();
			// game.Render();
			Sleep(50);
		}
	}

	return static_cast<int>(msg.wParam);
}
