#include "ntpch.h"
#include "RenderThread.h"
#include "Renderer/Renderer.h"

namespace Nut {

	struct RenderThreadData
	{
		CRITICAL_SECTION m_CriticalSection;
		CONDITION_VARIABLE m_ConditionVariable;

		RenderThread::State m_State = RenderThread::State::Idle;
	};
}

Nut::RenderThread::RenderThread(ThreadingPolicy threadingPolicy)
	: m_RenderThread("RenderThread"), m_ThreadingPolicy(threadingPolicy)
{
	m_Data = new RenderThreadData();

	if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
	{
		InitializeCriticalSection(&m_Data->m_CriticalSection);
		InitializeConditionVariable(&m_Data->m_ConditionVariable);
	}
}

Nut::RenderThread::~RenderThread()
{
	m_IsRunning = true;
	if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
	{
		DeleteCriticalSection(&m_Data->m_CriticalSection);
	}
}

void Nut::RenderThread::Run()
{
	m_IsRunning = true;
	if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
		m_RenderThread.Dispatch(Renderer::RenderThreadFunc, this);
}

void Nut::RenderThread::Terminate()
{
	m_IsRunning = false;
	Pump();

	if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
		m_RenderThread.Join();
}

void Nut::RenderThread::Wait(State waitForState)
{
	if (m_ThreadingPolicy == ThreadingPolicy::SingleThreaded)
		return;

	EnterCriticalSection(&m_Data->m_CriticalSection);
	while (m_Data->m_State != waitForState)
	{
		SleepConditionVariableCS(&m_Data->m_ConditionVariable, &m_Data->m_CriticalSection, INFINITE);
	}
	LeaveCriticalSection(&m_Data->m_CriticalSection);
}

void Nut::RenderThread::WaitAndSet(State waitForState, State setToState)
{
	if (m_ThreadingPolicy == ThreadingPolicy::SingleThreaded)
		return;

	EnterCriticalSection(&m_Data->m_CriticalSection);
	while (m_Data->m_State != waitForState)
	{
		SleepConditionVariableCS(&m_Data->m_ConditionVariable, &m_Data->m_CriticalSection, INFINITE);
	}
	m_Data->m_State = setToState;
	WakeAllConditionVariable(&m_Data->m_ConditionVariable);
	LeaveCriticalSection(&m_Data->m_CriticalSection);
}

void Nut::RenderThread::Set(State setToState)
{
	if (m_ThreadingPolicy == ThreadingPolicy::SingleThreaded)
		return;

	EnterCriticalSection(&m_Data->m_CriticalSection);
	m_Data->m_State = setToState;
	WakeAllConditionVariable(&m_Data->m_ConditionVariable);
	LeaveCriticalSection(&m_Data->m_CriticalSection);
}

void Nut::RenderThread::NextFrame()
{
	Renderer::SwapQueues();
}

void Nut::RenderThread::BlockUntilRenderComplete()
{
	if (m_ThreadingPolicy == ThreadingPolicy::SingleThreaded)
		return;

	Wait(State::Idle);
}

void Nut::RenderThread::Kick()
{
	if (m_ThreadingPolicy == ThreadingPolicy::MultiThreaded)
	{
		Set(State::Kick);
	}
	else
	{
		Renderer::WaitAndRender(this);
	}
}

void Nut::RenderThread::Pump()
{
	NextFrame();
	Kick();
	BlockUntilRenderComplete();
}
