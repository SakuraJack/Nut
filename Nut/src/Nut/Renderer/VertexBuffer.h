#pragma once
#include <glad/glad.h>
#include "Nut/Core/Buffer.h"

namespace Nut {
	enum class VertxBufferUsage
	{
		StaticDraw = 0,
		DynamicDraw,
		StreamDraw
	};

	enum class DataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t DataTypeSize(DataType type)
	{
		switch (type)
		{
		case Nut::DataType::Float:				return 4;
		case Nut::DataType::Float2:				return 4 * 2;
		case Nut::DataType::Float3:				return 4 * 3;
		case Nut::DataType::Float4:				return 4 * 4;
		case Nut::DataType::Mat3:				return 4 * 3 * 3;
		case Nut::DataType::Mat4:				return 4 * 4 * 4;
		case Nut::DataType::Int:				return 4;
		case Nut::DataType::Int2:				return 4 * 2;
		case Nut::DataType::Int3:				return 4 * 3;
		case Nut::DataType::Int4:				return 4 * 4;
		case Nut::DataType::Bool:				return 1;
		}
	}

	struct VertexBufferElements
	{
		std::string Name;
		DataType Type;
		uint32_t Size;
		uint32_t Offset;
		uint32_t Count; //  分量个数
		bool Normalized;

		VertexBufferElements() {}

		VertexBufferElements(const std::string& name, DataType type, bool normalized = false)
			: Name(name), Type(type), Size(DataTypeSize(type)), Offset(0), Normalized(normalized)
		{
			switch (type)
			{
				case DataType::Float:			Count = 1; break;
				case DataType::Float2:			Count = 2; break;
				case DataType::Float3:			Count = 3; break;
				case DataType::Float4:			Count = 4; break;
				case DataType::Mat3:			Count = 3 * 3; break;
				case DataType::Mat4:			Count = 4 * 4; break;
				case DataType::Int:				Count = 1; break;
				case DataType::Int2:			Count = 2; break;
				case DataType::Int3:			Count = 3; break;
				case DataType::Int4:			Count = 4; break;
				case DataType::Bool:			Count = 1; break;
			}
		}

		GLenum GetGLType() const
		{
			switch (Type)
			{
				case DataType::Float:			return GL_FLOAT;
				case DataType::Float2:			return GL_FLOAT;
				case DataType::Float3:			return GL_FLOAT;
				case DataType::Float4:			return GL_FLOAT;
				case DataType::Mat3:			return GL_FLOAT;
				case DataType::Mat4:			return GL_FLOAT;
				case DataType::Int:				return GL_INT;
				case DataType::Int2:			return GL_INT;
				case DataType::Int3:			return GL_INT;
				case DataType::Int4:			return GL_INT;
				case DataType::Bool:			return GL_BOOL;
			}
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout(){}
		VertexBufferLayout(const std::initializer_list<VertexBufferElements>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }  //  获取步长
		inline uint32_t GetCount() const { return m_Elements.size(); }	 //  获取元素个数
		inline const std::vector<VertexBufferElements>& GetElements() const { return m_Elements; } //  获取元素列表

		~VertexBufferLayout(){}

	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<VertexBufferElements> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(void* data, uint64_t size, VertxBufferUsage usage = VertxBufferUsage::StaticDraw);
		VertexBuffer(uint64_t size, VertxBufferUsage usage = VertxBufferUsage::DynamicDraw);
		~VertexBuffer();

		void SetData(void* data, uint64_t size, uint64_t offset = 0);
		void Bind() const;
		void Unbind() const;

		const VertexBufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const VertexBufferLayout& layout);

		RenderID GetBufferID() const { return m_BufferID; } //  获取缓冲区ID
		uint64_t GetSize() const { return m_Size; } //  获取缓冲区大小

	public:
		static std::shared_ptr<VertexBuffer> Create(void* data, uint64_t size, VertxBufferUsage usage = VertxBufferUsage::StaticDraw); //  创建顶点缓冲
		static std::shared_ptr<VertexBuffer> Create(uint64_t size, VertxBufferUsage usage = VertxBufferUsage::DynamicDraw); //  创建顶点缓冲

	private:
		RenderID m_BufferID = 0; //  缓冲区ID
		uint64_t m_Size = 0;	//  缓冲区大小 字节数
		VertxBufferUsage m_Usage; //  缓冲区使用方式
		VertexBufferLayout m_Layout; //  缓冲区布局
		Buffer m_LocalData;
	};
}