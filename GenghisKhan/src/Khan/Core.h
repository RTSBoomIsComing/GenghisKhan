#pragma once

#ifdef GK_PLATFORM_WINDOWS
	#ifdef GK_BUILD_DLL
		#define KHAN_API __declspec(dllexport)
	#else
		#define KHAN_API __declspec(dllimport)
	#endif
#else
	#error GenghisKhan only support Windows!
#endif