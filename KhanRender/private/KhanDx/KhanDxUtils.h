#pragma once

#include <winnt.h>
#include <winerror.h>
#include <string_view>
#include <source_location>

namespace KhanDx
{
	void ThrowIfFailed(HRESULT result, std::string_view message = "", std::source_location location = std::source_location::current());
	void ThrowIfFailed(BOOL result, std::string_view message, std::source_location location);
}
