#include "ntpch.h"
#include "VertexArray.h"
#include "Core/Log.h"
#include "glad/glad.h"

Nut::VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_VertexArrayID);
}

Nut::VertexArray::VertexArray(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glCreateVertexArrays(1, &m_VertexArrayID);
	if (vertexBuffer == nullptr)
	{
		NUT_ERROR_TAG("VertexArray", "¶¥µã»º³åÎª¿Õ");
		return;
	}
	AddVertexBuffer(vertexBuffer);
	if (indexBuffer == nullptr)
	{
		NUT_ERROR_TAG("VertexArray", "Ë÷Òý»º³åÎª¿Õ");
		return;
	}
	SetIndexBuffer(indexBuffer);
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
	int attributeIndex = 0;
	for (const auto& element : vertexBuffer->GetLayout().GetElements())
	{
		glEnableVertexArrayAttrib(m_VertexArrayID, attributeIndex);
		glVertexArrayAttribFormat(m_VertexArrayID, attributeIndex, element.Count, element.GetGLType(), element.Normalized, element.Offset);
		glVertexArrayAttribBinding(m_VertexArrayID, attributeIndex, 0);
		attributeIndex++;
	}
	m_VertexBuffers.push_back(vertexBuffer);
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
		m_VertexBuffers.push_back(vertexBuffer);
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
	m_IndexBuffer = indexBuffer;
}


