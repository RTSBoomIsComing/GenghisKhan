#include <Khan.h>

class Sandbox : public Khan::Application
{
public:
	Sandbox()
	{
	}
	~Sandbox()
	{ 
	}
	void Update()
	{
		Khan::Logger::Info("Updated");
	}
};

namespace Khan {
	std::unique_ptr<Application> CreateApplication()
	{
		auto sandbox = std::make_unique<Sandbox>();
		return std::move(sandbox);
	}
}