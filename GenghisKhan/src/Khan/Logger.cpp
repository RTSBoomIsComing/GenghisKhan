#include "pch.h"
#include "Logger.h"

#include <chrono>
#include <iostream>
#include <Windows.h>
#include <time.h>

enum class ConsoleColor
	{
		BLACK,
		BLUE,
		GREEN,
		AQUA,
		RED,
		PURPLE,
		YELLOW,
		WHITE,
		GRAY,
		LIGHT_BLUE,
		LIGHT_GREEN,
		LIGHT_AQUA,
		LIGHT_RED,
		LIGHT_PURPLE,
		LIGHT_YELLOW,
		BRIGHT_WHITE,
	};

namespace Khan {

    std::string Logger::CurrentDate()
    {
		using namespace std::chrono;
		std::time_t now = system_clock::to_time_t(system_clock::now());
		struct tm buf;
		localtime_s(&buf, &now);
		std::string output(30, '\0');
		std::strftime(output.data(), output.size(), "%y-%m-%d %H:%M:%S", &buf);

		return output;
    }
	void Logger::Info(const std::string_view message)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, static_cast<int>(ConsoleColor::LIGHT_GREEN));
		std::cout << "INFO: " << CurrentDate() << " - " << message << "\n";
		SetConsoleTextAttribute(hConsole, static_cast<int>(ConsoleColor::WHITE));
	}
	void Logger::Error(const std::string_view message, std::source_location location)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, static_cast<int>(ConsoleColor::LIGHT_RED));
		std::cout << "ERROR:" << CurrentDate() << " - " << message 
			<< " FILE:" << location.file_name() << " FUNC:" << location.function_name() 
			<< " LINE:" <<location.line() << "\n";
		SetConsoleTextAttribute(hConsole, static_cast<int>(ConsoleColor::WHITE));
	}
}
 