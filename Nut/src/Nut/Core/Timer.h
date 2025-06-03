#pragma once

#include <chrono>
#include "Log.h"

namespace Nut {

	class Timer
	{
	public:
		__forceinline Timer() { Reset(); }
		__forceinline void Reset() { m_Start = std::chrono::high_resolution_clock::now(); }
		__forceinline float Elapsed() const { return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f; }
		__forceinline float ElapsedMillis() const { return std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f; }

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

	class ScopedTimer
	{
	public:
		ScopedTimer(const std::string& name)
			: m_Name(name) {}
		~ScopedTimer() {
			float time = m_Timer.Elapsed();
			NUT_CORE_INFO_TAG("Timer", "{0} took {1}ms", m_Name, time);
		}

	private:
		Timer m_Timer;
		std::string m_Name;
	};
}
