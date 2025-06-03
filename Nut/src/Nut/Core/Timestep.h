#pragma once

namespace Nut {

	class Timestep
	{
	public:
		Timestep() {}
		Timestep(float time) : m_Time(time) {}

		inline float GetSeconds() const { return m_Time; } // ��ȡʱ�䲽��
		inline float GetMilliseconds() const { return m_Time * 1000.0f; } // ��ȡʱ�䲽�������룩

		operator float() { return m_Time; } // ת��Ϊ float ����

	private:
		float m_Time = 0.0f; // ʱ�䲽��
	};
}
