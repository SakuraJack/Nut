#include "ntpch.h"
#include "VertexArray.h"
#include "glad/glad.h"
#include "Core/Log.h"

Nut::VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_VertexArrayID);
}

Nut::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VertexArrayID);
}

void Nut::VertexArray::Bind() const
{
	glBindVertexArray(m_VertexArrayID);
}

void Nut::VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void Nut::VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	if (vertexBuffer == nullptr)
	{
		NUT_ERROR_TAG("VertexArray", "¶¥µã»º³åÎª¿Õ");
		return;
	}
	glVertexArrayVertexBuffer(m_VertexArrayID, 0, vertexBuffer->GetBufferID(), 0, vertexBuffer->GetLayout().GetStride());
}

void Nut::VertexArray::AddVertexBuffers(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers)
{
	int bufferCount = vertexBuffers.size();
	GLuint* buffers = new GLuint[bufferCount];
	GLintptr* offsets = new GLintptr[bufferCount];
	GLsizei* strides = new GLsizei[bufferCount];
	int index = 0;
	for (const auto& vertexBuffer : vertexBuffers) 
	{
		if (vertexBuffer == nullptr)
		{
			NUT_ERROR_TAG("VertexArray", "¶¥µã»º³åÎª¿Õ");
			continue;
		}
		buffers[index] = vertexBuffer->GetBufferID();
		offsets[index] = 0;
		strides[index] = vertexBuffer->GetLayout().GetStride();
	}
	glVertexArrayVertexBuffers(m_VertexArrayID, 0, bufferCount, buffers, offsets, strides);
}

void Nut::VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	if (indexBuffer == nullptr)
	{
		NUT_ERROR_TAG("VertexArray", "Ë÷Òý»º³åÎª¿Õ");
		return;
	}
	glVertexArrayElementBuffer(m_VertexArrayID, indexBuffer->GetBufferID());
}


