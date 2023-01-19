#pragma once
#include "pch.h"

#include "Game.h"

int main(int, char*[])
{
	KHAN_INFO("Run Genghis Khan");
	int result{};
	try
	{
		Game app{ 1280, 960, L"Genghis Khan(Sand box)" };
		result = app.Run();
	}
	catch (std::exception& e)
	{
		KHAN_ERROR(e.what()); 
		return 1;
	}
	return result;
}
