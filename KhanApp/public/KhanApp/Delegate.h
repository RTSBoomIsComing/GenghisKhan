#pragma once
#include <functional>
namespace KhanApp
{
	template<typename... Args>
	struct Delegate // Return types of funcions bound to Delegate are void
	{
		std::function<void(Args...)> DefaultFn{};
		std::function<void(Args...)> InstantFn{};

		void operator()(Args... args)
		{
			if (InstantFn)
			{
				InstantFn(args...);
				InstantFn = nullptr;
			}
			else if (DefaultFn)
			{
				DefaultFn(args...);
			}
		}
	};
}