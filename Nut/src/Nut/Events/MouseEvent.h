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
		int m_Button; // 按下的鼠标按钮 左中右
	};

	class MouseReleasedEvent : public Event
	{
	public:
		MouseReleasedEvent(int button)
			: m_Button(button) {}

		int GetButton() const { return m_Button; }
		EVENT_CLASS_TYPE(MouseButtonReleased);

	private:
		int m_Button; // 按下的鼠标按钮 左中右
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
		float m_X, m_Y; // 鼠标位置
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
		float m_XOffset, m_YOffset; // 鼠标滚轮滚动的距离
		// 可能需要添加鼠标滚轮的方向
		// 可能需要添加鼠标滚轮的速度
		// 可能需要添加鼠标滚轮的加速度
		// 可能需要添加鼠标滚轮的灵敏度
		// 可能需要添加鼠标滚轮的缩放比例
		// 可能需要添加鼠标滚轮的缩放速度
		// 可能需要添加鼠标滚轮的缩放加速度
		// 可能需要添加鼠标滚轮的缩放灵敏度
		// 可能需要添加鼠标滚轮的缩放范围
	};
}