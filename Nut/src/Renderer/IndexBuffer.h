#pragma once

namespace Nut {
	class IndexBuffer
	{
	public:
		IndexBuffer(void* data, unsigned int size, unsigned int count);
		IndexBuffer(unsigned int size, unsigned int count);
		~IndexBuffer();

		void SetData(void* data, unsigned int size, unsigned int offset = 0);
		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_Count; } //  ��ȡ��������
		inline unsigned int GetSize() const { return m_Size; } //  ��ȡ���������С
		inline unsigned int GetBufferID() const { return m_BufferID; } //  ��ȡ��������ID
		
	private:
		unsigned int m_BufferID;	//  ��������ID
		unsigned int m_Size;		//  ���������С
		unsigned int m_Count;		//  ��������
	};
}