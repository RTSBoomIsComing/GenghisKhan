#pragma once
#include "pch.h"
#include <Khan.h>

#include "Game.h"

int main(int, char* [])
{
	KHAN_INFO("Run Genghis Khan");
	int result{};

	std::unique_ptr<Khan::Application> game = std::make_unique<Game>() ;
	result = game->Run();

	return result;
}