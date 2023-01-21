#pragma once

int main(int, char* [])
{
	//KHAN_INFO("Run Genghis Khan");
	int result{};
	try
	{
		auto window = std::make_unique<Khan::Window>(1280, 960, L"Genghis Khan(Sand box)");

		Khan::Application app(window);
		result = app.Run();
	}
	catch (std::exception& e)
	{
		KHAN_ERROR(e.what());
		return 1;
	}
	return result;
}
