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

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer); // 绑定单个顶点缓冲 适合缓冲区不经常变化的情况 如模型数据
		void AddVertexBuffers(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers); // 绑定多个顶点缓冲 适合缓冲区经常变化的情况 如粒子系统
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer); // 绑定索引缓冲

	public:
		static std::shared_ptr<VertexArray> Create(); // 创建顶点数组对象
		static std::shared_ptr<VertexArray> Create(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer); // 创建顶点数组对象并绑定顶点缓冲和索引缓冲
	private:

		unsigned int m_VertexArrayID;	//  顶点数组对象ID
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers; //  顶点缓冲列表
		std::shared_ptr<IndexBuffer> m_IndexBuffer; //  索引缓冲

	};
}
