#pragma once

#include "Nut/Core/Buffer.h"

namespace Nut {
	class IndexBuffer
	{
	public:
		IndexBuffer(void* data, uint64_t size);
		IndexBuffer(uint64_t size);
		~IndexBuffer();

		void SetData(void* data, uint64_t size, uint64_t offset = 0);
		void Bind() const;
		void Unbind() const;

		inline uint64_t GetSize() const { return m_Size; } //  ��ȡ���������С
		inline uint64_t GetBufferID() const { return m_BufferID; } //  ��ȡ��������ID

	public:
		static std::shared_ptr<IndexBuffer> Create(void* data, uint64_t size); //  ������������
		static std::shared_ptr<IndexBuffer> Create(uint64_t size); //  ������������
		
	private:
		RenderID m_BufferID = 0;	//  ��������ID
		uint64_t m_Size = 0;		//  ���������С
		Buffer m_LocalData;
	};
}