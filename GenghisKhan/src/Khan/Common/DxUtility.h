#pragma once

#include <winnt.h>
#include <winerror.h>
#include <string_view>
#include "Logger.h"

namespace Khan {
	inline void ThrowIfFailed(HRESULT hr, std::string_view message = "", std::source_location location = std::source_location::current())
	{
		if (FAILED(hr))
		{
			KHAN_ERROR(message, location);
			throw hr;
		}
	}
}
