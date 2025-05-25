#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Nut {
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer);
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer); // �󶨵������㻺��
		void AddVertexBuffers(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers); // �󶨶�����㻺��
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer); // ����������

		RenderID GetID() const { return m_VertexArrayID; } // ��ȡ�����������ID

	public:
		static std::shared_ptr<VertexArray> Create(); // ���������������
		static std::shared_ptr<VertexArray> Create(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer); // ��������������󲢰󶨶��㻺�����������
	private:

		RenderID m_VertexArrayID;	//  �����������ID
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers; //  ���㻺���б�
		std::shared_ptr<IndexBuffer> m_IndexBuffer; //  ��������
	};
}
