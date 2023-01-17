#include "Application.h"

namespace Khan {
	
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		
		for (int i{}; i < 10; ++i)
		{
			Update();
		}
	}
}