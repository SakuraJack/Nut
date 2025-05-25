#pragma once

#include "Log.h"


#ifdef NT_PLATFORM_WINDOWS
#define NT_DEBUG_BREAK __debugbreak()
#else
#define NT_DEBUG_BREAK
#endif

#ifdef NT_DEBUG
#define NT_ENABLE_ASSERTS
#endif

#ifdef NT_ENABLE_ASSERTS
#define NT_ASSERT(condition, ...) { if(!(condition)) { NUT_ERROR("Assertion Failed: {0}", __VA_ARGS__); NT_DEBUG_BREAK; } }
#else 
#define NT_ASSERT(condition, ...)
#endif