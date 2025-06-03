#pragma once
#include "Event.h"

namespace Nut {
	class MousePressedEvent : public Event
	{
	public:
		MousePressedEvent(int button)
			: m_Button(button) {}

		int GetButton() const { return m_Button; }
		EVENT_CLASS_TYPE(MouseButtonPressed);

	private:
		int m_Button; // ���µ���갴ť ������
	};

	class MouseReleasedEvent : public Event
	{
	public:
		MouseReleasedEvent(int button)
			: m_Button(button) {}

		int GetButton() const { return m_Button; }
		EVENT_CLASS_TYPE(MouseButtonReleased);

	private:
		int m_Button; // ���µ���갴ť ������
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_X(x), m_Y(y) {}

		float GetX() const { return m_X; }
		float GetY() const { return m_Y; }
		EVENT_CLASS_TYPE(MouseMoved);

	private:
		float m_X, m_Y; // ���λ��
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }
		EVENT_CLASS_TYPE(MouseScrolled);

	private:
		float m_XOffset, m_YOffset; // �����ֹ����ľ���
		// ������Ҫ��������ֵķ���
		// ������Ҫ��������ֵ��ٶ�
		// ������Ҫ��������ֵļ��ٶ�
		// ������Ҫ��������ֵ�������
		// ������Ҫ��������ֵ����ű���
		// ������Ҫ��������ֵ������ٶ�
		// ������Ҫ��������ֵ����ż��ٶ�
		// ������Ҫ��������ֵ�����������
		// ������Ҫ��������ֵ����ŷ�Χ
	};
}