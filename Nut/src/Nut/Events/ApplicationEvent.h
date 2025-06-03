#pragma once
#include "Event.h"

namespace Nut {
	class WindowResizeEvent : public Event
	{
		// 窗口大小改变时触发
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }
		EVENT_CLASS_TYPE(WindowResized);
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
		// 窗口关闭时触发
	public:
		WindowCloseEvent() {}
		~WindowCloseEvent() = default;
		EVENT_CLASS_TYPE(WindowClosed);
	};

	class WindowMoveEvent : public Event
	{
		// 窗口移动时触发
	public:
		WindowMoveEvent(int x, int y)
			: m_X(x), m_Y(y) {}

		int GetX() const { return m_X; }
		int GetY() const { return m_Y; }
		EVENT_CLASS_TYPE(WindowMoved);
	private:
		int m_X, m_Y;
		// 窗口位置
		// 可能需要添加窗口大小
		// 可能需要添加窗口标题
		// 可能需要添加窗口图标
		// 可能需要添加窗口状态
		// 可能需要添加窗口样式
		// 可能需要添加窗口透明度
		// 可能需要添加窗口最小化/最大化/还原状态
	};

	class WindowFocusEvent : public Event
	{
		// 当窗口获得焦点时触发
	public:
		WindowFocusEvent() {}
		~WindowFocusEvent() = default;
		EVENT_CLASS_TYPE(WindowFocus);
	};

	class WindowLostFocusEvent : public Event
	{
		// 当窗口失去焦点时触发
	public:
		WindowLostFocusEvent() {}
		~WindowLostFocusEvent() = default;
		EVENT_CLASS_TYPE(WindowLostFocus);
	};
}