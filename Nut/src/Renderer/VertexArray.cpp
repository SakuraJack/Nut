#include "ntpch.h"
#include "VertexArray.h"
#include "Core/Log.h"
#include "glad/glad.h"
#include "Renderer.h"

Nut::VertexArray::VertexArray()
{
	Renderer::Submit([this]() {
		glCreateVertexArrays(1, &m_VertexArrayID);
		});
}

Nut::VertexArray::VertexArray(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	Renderer::Submit([this]() {
		glCreateVertexArrays(1, &m_VertexArrayID);
		});
	AddVertexBuffer(vertexBuffer);
	SetIndexBuffer(indexBuffer);
}

Nut::VertexArray::~VertexArray()
{
	RenderID vertexArrayID = m_VertexArrayID;
	Renderer::Submit([vertexArrayID]() {
		glDeleteVertexArrays(1, &vertexArrayID);
		});
}

void Nut::VertexArray::Bind() const
{
	Renderer::Submit([this]() {
		glBindVertexArray(m_VertexArrayID);
		});
}

void Nut::VertexArray::Unbind() const
{
	Renderer::Submit([]() {
		glBindVertexArray(0);
		});
}

void Nut::VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	if (vertexBuffer == nullptr)
	{
		NUT_ERROR_TAG("VertexArray", "¶¥µã»º³åÎª¿Õ");
		return;
	}
	m_VertexBuffers.push_back(vertexBuffer);
	Renderer::Submit([this]() {
		glVertexArrayVertexBuffer(m_VertexArrayID, 0, m_VertexBuffers.back()->GetBufferID(), 0, m_VertexBuffers.back()->GetLayout().GetStride());
		int attributeIndex = 0;
		for (const auto& element : m_VertexBuffers.back()->GetLayout().GetElements())
		{
			glEnableVertexArrayAttrib(m_VertexArrayID, attributeIndex);
			glVertexArrayAttribFormat(m_VertexArrayID, attributeIndex, element.Count, element.GetGLType(), element.Normalized, element.Offset);
			glVertexArrayAttribBinding(m_VertexArrayID, attributeIndex, 0);
			attributeIndex++;
		}
		});
}

void Nut::VertexArray::AddVertexBuffers(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers)
{
	// TODO: ¶à»º³å°ó¶¨
	/*int bufferCount = vertexBuffers.size();
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
	glVertexArrayVertexBuffers(m_VertexArrayID, 0, bufferCount, buffers, offsets, strides);*/
}

void Nut::VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	if (indexBuffer == nullptr)
	{
		NUT_ERROR_TAG("VertexArray", "Ë÷Òý»º³åÎª¿Õ");
		return;
	}
	m_IndexBuffer = indexBuffer;
	Renderer::Submit([this]() {
		glVertexArrayElementBuffer(m_VertexArrayID, m_IndexBuffer->GetBufferID());
		});
}

std::shared_ptr<Nut::VertexArray> Nut::VertexArray::Create()
{
	return std::make_shared<VertexArray>();
}

std::shared_ptr<Nut::VertexArray> Nut::VertexArray::Create(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	return std::make_shared<VertexArray>(vertexBuffer, indexBuffer);
}


