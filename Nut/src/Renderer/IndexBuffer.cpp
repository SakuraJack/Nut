#include "ntpch.h"
#include "IndexBuffer.h"
#include "Core/Log.h"
#include "glad/glad.h"

Nut::IndexBuffer::IndexBuffer(void* data, unsigned int size, unsigned int count)
	: m_Size(size), m_Count(count)
{
	glCreateBuffers(1, &m_BufferID);
	glNamedBufferData(m_BufferID, size, data, GL_STATIC_DRAW);
}

Nut::IndexBuffer::IndexBuffer(unsigned int size, unsigned int count)
	: m_Size(size), m_Count(count)
{
	glCreateBuffers(1, &m_BufferID);
	glNamedBufferData(m_BufferID, size, nullptr, GL_DYNAMIC_DRAW);
}


Nut::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_BufferID);
}

void Nut::IndexBuffer::SetData(void* data, unsigned int size, unsigned int offset)
{
	glNamedBufferSubData(m_BufferID, offset, size, data);
}

void Nut::IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
}

void Nut::IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
