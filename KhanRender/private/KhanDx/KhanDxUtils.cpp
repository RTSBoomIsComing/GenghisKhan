#include "pch.h"
#include "KhanDxUtils.h"
#include "KhanTools/Log.h"

void KhanDx::ThrowIfFailed(HRESULT result, std::string_view message, std::source_location location)
{
	if (FAILED(result))
	{
		KHAN_ERROR(message, location);
		throw result;
	}
}

void KhanDx::ThrowIfFailed(BOOL result, std::string_view message, std::source_location location)
{
	if (result == 0)
	{
		KHAN_ERROR(message, location);
		throw GetLastError();
	}
}