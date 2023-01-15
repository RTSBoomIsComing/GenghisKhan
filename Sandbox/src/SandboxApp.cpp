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

};
#include <memory>
int main()
{
	auto sandbox = std::make_unique<Sandbox>();
	sandbox->Run();
	return 0;
}