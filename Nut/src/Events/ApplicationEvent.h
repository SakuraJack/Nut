#pragma once
#include "Event.h"

namespace Nut {
	class WindowResizeEvent : public Event
	{
		// ���ڴ�С�ı�ʱ����
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
		// ���ڹر�ʱ����
	public:
		WindowCloseEvent() {}
		~WindowCloseEvent() = default;
		EVENT_CLASS_TYPE(WindowClosed);
	};

	class WindowMoveEvent : public Event
	{
		// �����ƶ�ʱ����
	public:
		WindowMoveEvent(int x, int y)
			: m_X(x), m_Y(y) {}

		int GetX() const { return m_X; }
		int GetY() const { return m_Y; }
		EVENT_CLASS_TYPE(WindowMoved);
	private:
		int m_X, m_Y;
		// ����λ��
		// ������Ҫ��Ӵ��ڴ�С
		// ������Ҫ��Ӵ��ڱ���
		// ������Ҫ��Ӵ���ͼ��
		// ������Ҫ��Ӵ���״̬
		// ������Ҫ��Ӵ�����ʽ
		// ������Ҫ��Ӵ���͸����
		// ������Ҫ��Ӵ�����С��/���/��ԭ״̬
	};

	class WindowFocusEvent : public Event
	{
		// �����ڻ�ý���ʱ����
	public:
		WindowFocusEvent() {}
		~WindowFocusEvent() = default;
		EVENT_CLASS_TYPE(WindowFocus);
	};

	class WindowLostFocusEvent : public Event
	{
		// ������ʧȥ����ʱ����
	public:
		WindowLostFocusEvent() {}
		~WindowLostFocusEvent() = default;
		EVENT_CLASS_TYPE(WindowLostFocus);
	};
}