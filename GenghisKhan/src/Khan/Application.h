#pragma once

namespace Khan {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		virtual void Update() abstract;
	};

	// To be defined in CLIENT
	// Application* CreateApplication();
}

