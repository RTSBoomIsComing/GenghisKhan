#pragma once
#include <memory>

namespace Khan {
	std::unique_ptr<Application> CreateApplication(); // To be defined in CLIENT
}
int main(int argc, char** argv)
{ 
	Khan::Logger::Info("Run Genghis Khan");
	Khan::Logger::Error("This is Error!");
	auto app = Khan::CreateApplication();
	app->Run();
	
	return 0;
}
