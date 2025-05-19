#pragma once

#include "Thread.h"
#include <atomic>

namespace Nut {

	struct RenderThreadData;

	enum class ThreadingPolicy
	{
		None = 0, SingleThreaded, MultiThreaded
	};

	class RenderThread
	{
	public:
		enum class State
		{
			Idle = 0,
			Busy,
			Kick
		};

	public:
		RenderThread(ThreadingPolicy threadingPolicy);
		~RenderThread();

		void Run();
		bool IsRunning() const { return m_IsRunning; }
		void Terminate();

		void Wait(State waitForState);
		void WaitAndSet(State waitForState, State setToState);
		void Set(State setToState);

		void NextFrame();
		void BlockUntilRenderComplete();
		void Kick();

		void Pump();

		ThreadingPolicy GetThreadingPolicy() const { return m_ThreadingPolicy; }
	private:
		RenderThreadData* m_Data;
		ThreadingPolicy m_ThreadingPolicy;

		Thread m_RenderThread;

		bool m_IsRunning = false;
	};
}