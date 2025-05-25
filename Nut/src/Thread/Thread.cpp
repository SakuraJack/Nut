#include "ntpch.h"
#include "Thread.h"

Nut::Thread::Thread(const std::string& name)
	: m_Name(name)
{

}

void Nut::Thread::SetName(const std::string& name)
{
	HANDLE threadHandle = m_Thread.native_handle();

	std::wstring wName(name.begin(), name.end());
	SetThreadDescription(threadHandle, wName.c_str());
	SetThreadAffinityMask(threadHandle, 0);
}

void Nut::Thread::Join()
{
	m_Thread.join();
}

Nut::ThreadSignal::ThreadSignal(const std::string& name, bool manualReset /*= false*/)
{
	std::wstring str(name.begin(), name.end());
	m_SignalHandle = CreateEvent(NULL, (BOOL)manualReset, FALSE, str.c_str());
}

void Nut::ThreadSignal::Wait()
{
	WaitForSingleObject(m_SignalHandle, INFINITE);
}

void Nut::ThreadSignal::Signal()
{
	SetEvent(m_SignalHandle);
}

void Nut::ThreadSignal::Reset()
{
	ResetEvent(m_SignalHandle);
}
