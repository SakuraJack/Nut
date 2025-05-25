#pragma once

namespace Nut {
	// 全局唯一标识符 64位|32位随机数生成 

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other);

		operator uint64_t() { return m_UUID; }
		operator const uint64_t() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};

	class UUID32
	{
	public:
		UUID32();
		UUID32(uint32_t uuid);
		UUID32(const UUID32& other);

		operator uint32_t() { return m_UUID; }
		operator const uint32_t() const { return m_UUID; }

	private:
		uint32_t m_UUID;
	};
}