#pragma once
#include "pch.h"


#include <Khan.h>

int main(int, char**)
{
	KHAN_INFO("Run Genghis Khan");
	Khan::Application game{ 1280, 960, L"test_Game" };
	return game.Run();
}
