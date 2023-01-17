#pragma once

#include <winnt.h>
#include <winerror.h>
#include <string_view>
#include "Logger.h"

inline void ThrowIfFailed(HRESULT hr, std::string_view message = "")
{
	if (FAILED(hr))
	{
		KHAN_ERROR(message);
		throw hr;
	}
}
