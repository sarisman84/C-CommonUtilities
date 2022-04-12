// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#ifdef COMMON_UTILITIES_EXPORTS
#define CU_API __declspec(dllexport)
#else
#define CU_API __declspec(dllimport)
#endif

/*
#ifndef CU_LIB
#define CU_LIB
#ifdef _DEBUG
#pragma comment(lib,"Common_Utilities_Debug.lib")
#else
#pragma comment(lib,"Common_Utilities_Release.lib")
#endif // _DEBUG
#endif // !CU_LIB
*/


// reference additional headers your program requires here
