#pragma once

#include <memory>

namespace Nut {

	void InitializeCore();
	void ShutdownCore();
}

#if !defined(NT_PLATFORM_WINDOWS)
	#error Unknown platform! Nut only supports Windows at the moment.
#endif

#define BIT(x) (1u << x)

#define NUT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Nut {

	using byte = uint8_t;
	using RenderID = uint32_t;
}