#include "ntpch.h"
#include "glad/glad.h"

#include "IndexBuffer.h"
#include "Core/Log.h"
#include "Renderer.h"

Nut::IndexBuffer::IndexBuffer(void* data, uint64_t size)
	: m_Size(size)
{
	m_LocalData.Copy(data, size);
	Renderer::Submit([this]() {
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, m_LocalData.Size, m_LocalData.Data, GL_STATIC_DRAW);
		});
}

Nut::IndexBuffer::IndexBuffer(uint64_t size)
	: m_Size(size)
{
	m_LocalData.Allocate(size);
	Renderer::Submit([this]() {
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, m_LocalData.Size, nullptr, GL_DYNAMIC_DRAW);
		});
}


Nut::IndexBuffer::~IndexBuffer()
{
	RenderID bufferID = m_BufferID;
	Renderer::Submit([bufferID]() {
		glDeleteBuffers(1, &bufferID);
		});
}

void Nut::IndexBuffer::SetData(void* data, uint64_t size, uint64_t offset)
{
	m_LocalData.Write(data, size, offset);
	Renderer::Submit([this, size, offset]() {
		glNamedBufferSubData(m_BufferID, offset, size, (byte*)m_LocalData.Data + offset);
		});
}

void Nut::IndexBuffer::Bind() const
{
	Renderer::Submit([this]() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		});
}

void Nut::IndexBuffer::Unbind() const
{
	Renderer::Submit([]() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		});
}

std::shared_ptr<Nut::IndexBuffer> Nut::IndexBuffer::Create(void* data, uint64_t size)
{
	return std::make_shared<IndexBuffer>(data, size);
}

std::shared_ptr<Nut::IndexBuffer> Nut::IndexBuffer::Create(uint64_t size)
{
	return std::make_shared<IndexBuffer>(size);
}
