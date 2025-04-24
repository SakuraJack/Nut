#pragma once
#include "Event.h"

namespace Nut {
	class KeyPressedEvent :public Event
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: m_KeyCode(keycode), m_RepeatCount(repeatCount) {}

		int GetKeyCode() const { return m_KeyCode; }
		int GetRepeatCount() const { return m_RepeatCount; }
		EVENT_CLASS_TYPE(KeyPressed);

	private:
		int m_KeyCode; // ���µļ���
		int m_RepeatCount; // �ظ����µĴ���
	};

	class KeyReleasedEvent :public Event
	{
	public:
		KeyReleasedEvent(int keycode)
			: m_KeyCode(keycode) {}

		int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_TYPE(KeyReleased);
	private:
		int m_KeyCode; // ���µļ���
	};

	class KeyTypedEvent :public Event
	{
	public:
		KeyTypedEvent(int keycode)
			: m_KeyCode(keycode) {}

		int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_TYPE(KeyTyped);
	private:
		int m_KeyCode; // ���µļ���
		// ������Ҫ��Ӱ��µ��ַ�
		// ������Ҫ��Ӱ��µļ��̲���
		// ������Ҫ��Ӱ��µļ���״̬
		// ������Ҫ��Ӱ��µļ�������
		// ������Ҫ��Ӱ��µļ�������
		// ������Ҫ��Ӱ��µļ������뷨
	};
}
