#pragma once

#include <string_view>
#include <source_location>

namespace Khan {

	class Logger
	{
	private:
		static std::string CurrentDate();
	private:
		Logger() noexcept = default;
		~Logger() noexcept = default;
	public:
		static void Info(const std::string_view message);
		static void Error(const std::string_view message, std::source_location location = std::source_location::current());
	};
}

#define KHAN_ERROR(...) Khan::Logger::Error(__VA_ARGS__)
#define KHAN_INFO(...) Khan::Logger::Info(__VA_ARGS__)