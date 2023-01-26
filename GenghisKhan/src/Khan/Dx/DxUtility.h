#pragma once

#include <winnt.h>
#include <winerror.h>
#include <string_view>
#include <source_location>

namespace Khan {
	void ThrowIfFailed(HRESULT hr, std::string_view message = "", std::source_location location = std::source_location::current());
}
