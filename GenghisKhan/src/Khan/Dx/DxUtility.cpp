#include "pch.h"
#include "DxUtility.h"
#include "Common/Logger.h"
void Khan::ThrowIfFailed(HRESULT hr, std::string_view message, std::source_location location)
{
	if (FAILED(hr))
	{
		KHAN_ERROR(message, location);
		throw hr;
	}
}
