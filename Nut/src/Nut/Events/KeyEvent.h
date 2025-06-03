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
		int m_KeyCode; // 按下的键码
		int m_RepeatCount; // 重复按下的次数
	};

	class KeyReleasedEvent :public Event
	{
	public:
		KeyReleasedEvent(int keycode)
			: m_KeyCode(keycode) {}

		int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_TYPE(KeyReleased);
	private:
		int m_KeyCode; // 按下的键码
	};

	class KeyTypedEvent :public Event
	{
	public:
		KeyTypedEvent(int keycode)
			: m_KeyCode(keycode) {}

		int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_TYPE(KeyTyped);
	private:
		int m_KeyCode; // 按下的键码
		// 可能需要添加按下的字符
		// 可能需要添加按下的键盘布局
		// 可能需要添加按下的键盘状态
		// 可能需要添加按下的键盘类型
		// 可能需要添加按下的键盘语言
		// 可能需要添加按下的键盘输入法
	};
}
