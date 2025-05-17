#include "ntpch.h"
#include "VertexBuffer.h"
#include "Core/Log.h"
#include "glad/glad.h"

#include "Renderer/Renderer.h"

Nut::VertexBuffer::VertexBuffer(void* data, uint64_t size, VertxBufferUsage usage)
	: m_Size(size)
{
	m_LocalData.Copy(data, size);
	m_Usage = usage;
	Renderer::Submit([this]() {
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, m_Size, m_LocalData.Data, GL_STATIC_DRAW);
		});
}

Nut::VertexBuffer::VertexBuffer(uint64_t size, VertxBufferUsage usage)
	: m_Size(size), m_Usage(usage)
{
	m_LocalData.Allocate(size);
	m_Usage = usage;
	Renderer::Submit([this]() {
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferData(m_BufferID, m_Size, nullptr, GL_DYNAMIC_DRAW);
		});
}

Nut::VertexBuffer::~VertexBuffer()
{
	RenderID bufferID = m_BufferID;
	Renderer::Submit([bufferID]() {
		glDeleteBuffers(1, &bufferID);
		});
}

void Nut::VertexBuffer::SetData(void* data, uint64_t size, uint64_t offset)
{
	m_LocalData.Write(data, size, offset);
	Renderer::Submit([this, size, offset]() {
		glNamedBufferSubData(m_BufferID, offset, size, (byte*)m_LocalData.Data + offset);
		});
}

void Nut::VertexBuffer::Bind() const
{
	Renderer::Submit([this]() {
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		});
}

void Nut::VertexBuffer::Unbind() const
{
	Renderer::Submit([this]() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		});
}

void Nut::VertexBuffer::SetLayout(const VertexBufferLayout& layout)
{
	m_Layout = layout;
}

std::shared_ptr<Nut::VertexBuffer> Nut::VertexBuffer::Create(void* data, uint64_t size, VertxBufferUsage usage /*= VertxBufferUsage::StaticDraw*/)
{
	return std::make_shared<VertexBuffer>(data, size, usage);
}

std::shared_ptr<Nut::VertexBuffer> Nut::VertexBuffer::Create(uint64_t size, VertxBufferUsage usage /*= VertxBufferUsage::DynamicDraw*/)
{
	return std::make_shared<VertexBuffer>(size, usage);
}
