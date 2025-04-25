#pragma once

namespace Nut {
	enum class VertxBufferUsage
	{
		StaticDraw = 0,
		DynamicDraw,
		StreamDraw
	};

	enum class DataType
	{
		None = 0, Float, Int, Vec2, Vec3, Vec4, Mat2, Mat3, Mat4, Bool
	};

	static unsigned int DataTypeSize(DataType type)
	{
		switch (type)
		{
			// ռ���ֽ���
		case DataType::Float:   return 4;
		case DataType::Int:     return 4;
		case DataType::Vec2:    return 2 * 4;
		case DataType::Vec3:    return 3 * 4;
		case DataType::Vec4:    return 4 * 4;
		case DataType::Mat2:    return 2 * 2 * 4;
		case DataType::Mat3:    return 3 * 3 * 4;
		case DataType::Mat4:    return 4 * 4 * 4;
		case DataType::Bool:    return 1;
		}
		return 0;
	}

	struct VertexBufferElements
	{
		std::string Name;
		DataType Type;
		unsigned int Size;
		unsigned int Offset;
		bool Normalized;

		VertexBufferElements() {}

		VertexBufferElements(const std::string& name, DataType type, bool normalized = false)
			: Name(name), Type(type), Size(DataTypeSize(type)), Offset(0), Normalized(normalized)
		{}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout(){}
		VertexBufferLayout(const std::vector<VertexBufferElements>& elements)
			: m_Elements(elements)
		{}

		inline unsigned int GetStride() const { return m_Stride; }  //  ��ȡ����
		inline unsigned int GetCount() const { return m_Elements.size(); }	 //  ��ȡԪ�ظ���

		~VertexBufferLayout(){}

	private:
		void CalculateOffsetsAndStride()
		{
			unsigned int offset = 0;
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
		unsigned int m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(void* data, unsigned int size, VertxBufferUsage usage = VertxBufferUsage::StaticDraw);
		VertexBuffer(unsigned int size, VertxBufferUsage usage = VertxBufferUsage::DynamicDraw);
		~VertexBuffer();

		void SetData(void* data, unsigned int size, unsigned int offset = 0);
		void Bind() const;
		void Unbind() const;

		const VertexBufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const VertexBufferLayout& layout);

		unsigned int GetSize() const { return m_Size; } //  ��ȡ��������С
		unsigned int GetBufferID() const { return m_BufferID; } //  ��ȡ������ID
		unsigned int GetCount() const { return m_Count; } //  ��ȡ������Ԫ�ظ���

	private:
		unsigned int m_BufferID; //  ������ID
		unsigned int m_Size;	//  ��������С
		unsigned int m_Count;	//  ������Ԫ�ظ���
		VertxBufferUsage m_Usage; //  ������ʹ�÷�ʽ
		VertexBufferLayout m_Layout; //  ����������
	};
}