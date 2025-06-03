#pragma once

namespace Nut {

	class Timestep
	{
	public:
		Timestep() {}
		Timestep(float time) : m_Time(time) {}

		inline float GetSeconds() const { return m_Time; } // 获取时间步长
		inline float GetMilliseconds() const { return m_Time * 1000.0f; } // 获取时间步长（毫秒）

		operator float() { return m_Time; } // 转换为 float 类型

	private:
		float m_Time = 0.0f; // 时间步长
	};
}
