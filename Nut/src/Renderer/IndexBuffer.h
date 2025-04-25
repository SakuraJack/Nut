#pragma once

namespace Nut {
	class IndexBuffer
	{
	public:
		IndexBuffer(unsigned int size);
		IndexBuffer(void* data, unsigned int size);
		~IndexBuffer();

		void SetData(void* data, unsigned int size, unsigned int offset = 0);
		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_Count; } //  获取索引个数
		inline unsigned int GetSize() const { return m_Size; } //  获取索引缓冲大小
		inline unsigned int GetBufferID() const { return m_BufferID; } //  获取索引缓冲ID
		
	private:
		unsigned int m_BufferID;
		unsigned int m_Size;
		unsigned int m_Count;
	};
}