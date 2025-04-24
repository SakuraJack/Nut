#pragma once
#include "ntpch.h"

namespace Nut {
	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResized, WindowMoved, WindowFocus, WindowLostFocus,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override{ return #type; }

	class Event
	{
	public:
		bool m_Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFN = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		};

		template<typename T>
		bool Dispatch(EventFN<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};
}
