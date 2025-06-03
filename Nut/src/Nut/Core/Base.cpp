#include "ntpch.h"
#include "Base.h"
#include "Log.h"
#include "Memory.h"

namespace Nut {

	void InitializeCore()
	{
		Allocator::Init();
		Log::Init();

		NUT_CORE_TRACE_TAG("Core", "Nut Engine");
		NUT_CORE_TRACE_TAG("Core", "Initializing...");
	}

	void ShutdownCore()
	{
		NUT_CORE_TRACE_TAG("Core", "Shutting down...");
		Log::Shutdown();
	}
}