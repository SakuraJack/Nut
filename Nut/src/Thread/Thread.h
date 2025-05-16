#pragma once

namespace Nut {

	class Thread
	{
	public:
		Thread(const std::string& name);

		template<typename Func, typename... Args>
		void Dispatch(Func&& func, Args&&... args)
		{
			m_Thread = std::thread(func, std::forward<Args>(args)...);
			SetName(m_Name);
		}

		void SetName(const std::string& name);

		void Join();

	private:
		std::string m_Name;
		std::thread m_Thread;
	};

	class ThreadSignal
	{
	public:
		ThreadSignal(const std::string& name, bool manualReset = false);

		void Wait();
		void Signal();
		void Reset();

	private:
		void* m_SignalHandle = nullptr;
	};
}