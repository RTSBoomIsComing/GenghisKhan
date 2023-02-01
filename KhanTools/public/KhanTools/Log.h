#pragma once

#include <string>
#include <string_view>
#include <source_location>

#ifdef KHANTOOLS_EXPORTS
#define KHAN_API __declspec(dllexport)
#else
#define KHAN_API __declspec(dllimport)
#endif

namespace KhanTools
{
	std::string GetCurrentDate();

	KHAN_API void Info(const std::string_view message);
	KHAN_API void Error(const std::string_view message, std::source_location location = std::source_location::current());

}

#define KHAN_ERROR(...)	KhanTools::Error(__VA_ARGS__)
#define KHAN_INFO(...)	KhanTools::Info(__VA_ARGS__)