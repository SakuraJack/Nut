#include "ntpch.h"
#include "VertexBuffer.h"
#include "Core/Log.h"
#include "glad/glad.h"

Nut::VertexBuffer::VertexBuffer(void* data, unsigned int size, unsigned int count, VertxBufferUsage usage)
	: m_Size(size), m_Count(count)
{
	m_Usage = usage;
	glCreateBuffers(1, &m_BufferID);
	glNamedBufferData(m_BufferID, size, data, GL_STATIC_DRAW);
}

Nut::VertexBuffer::VertexBuffer(unsigned int size, unsigned int count, VertxBufferUsage usage)
	: m_Size(size), m_Usage(usage), m_Count(count)
{
	glCreateBuffers(1, &m_BufferID);
	glNamedBufferData(m_BufferID, size, nullptr, GL_DYNAMIC_DRAW);
}

Nut::VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_BufferID);
}

void Nut::VertexBuffer::SetData(void* data, unsigned int size, unsigned int offset)
{
	glNamedBufferSubData(m_BufferID, offset, size, data);
}

void Nut::VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
}

void Nut::VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Nut::VertexBuffer::SetLayout(const VertexBufferLayout& layout)
{
	m_Layout = layout;
}

std::shared_ptr<Nut::VertexBuffer> Nut::VertexBuffer::Create(void* data, unsigned int size, unsigned int count, VertxBufferUsage usage /*= VertxBufferUsage::StaticDraw*/)
{
	return std::make_shared<VertexBuffer>(data, size, count, usage);
}

std::shared_ptr<Nut::VertexBuffer> Nut::VertexBuffer::Create(unsigned int size, unsigned int count, VertxBufferUsage usage /*= VertxBufferUsage::DynamicDraw*/)
{
		return std::make_shared<VertexBuffer>(size, count, usage);
}
