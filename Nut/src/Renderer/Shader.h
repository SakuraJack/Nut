#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"

namespace Nut {
	enum class ShaderUniformType
	{
		None = 0, Bool, Int, Float, Vec2, Vec3, Vec4, Mat3, Mat4,
	};

	class ShaderUniform
	{
	public:
		ShaderUniform() = default;
		ShaderUniform(const std::string& name, ShaderUniformType type, unsigned int size, unsigned int offset)
			: Name(name), Type(type), Size(size), Offset(offset) {}

		const std::string& GetName() const { return Name; }	//  ��ȡ����
		ShaderUniformType GetType() const { return Type; }	//  ��ȡ����
		unsigned int GetSize() const { return Size; }	//  ��ȡ��С
		unsigned int GetOffset() const { return Offset; }	//  ��ȡƫ����

	private:
		std::string Name;	//  ����
		ShaderUniformType Type;	//  ����
		unsigned int Size;	//  ��С
		unsigned int Offset;	//  ƫ����
	};

	struct ShaderUniformBuffer {
		std::string Name;	//  ����
		unsigned int Size;	//  ��С
		unsigned int Binding;	//  �󶨵�
		unsigned int BufferID;	//  ������ID
		std::unordered_map<std::string, ShaderUniform> Uniforms;	//  Uniform�����б�
	};

	struct ShaderStorageBuffer {

	};

	class Shader
	{
	public:
		Shader(const std::string name = "DefaultShader");
		Shader(const std::string& name, const std::string& shaderSourcePath);	//  ���캯��

		void Reload(const std::string& shaderSourcePath);	//  ������ɫ��

		void Bind();		//  ����ɫ��
		void Unbind();		//  �����ɫ��

		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, const glm::ivec2 value);
		void SetUniform(const std::string& name, const glm::vec2 value);
		void SetUniform(const std::string& name, const glm::ivec3 value);
		void SetUniform(const std::string& name, const glm::vec3 value);
		void SetUniform(const std::string& name, const glm::ivec4 value);
		void SetUniform(const std::string& name, const glm::vec4 value);
		void SetUniform(const std::string& name, const glm::mat3 value);
		void SetUniform(const std::string& name, const glm::mat4 value);

		static ShaderUniformBuffer& GetUniformBuffer(const std::string& uniformBufferName);	//  ��ȡUniform������
		static void SetUniformBuffer(const std::string& uniformBufferName, const void* data, unsigned int size, unsigned int offset = 0);	//  ����Uniform������
		static void ClearUniformBuffers();	//  ���Uniform������

		// TODO: ֧��StorageBuffer
		//static void SetStorageBuffer(const std::string& storageBufferName, const ShaderStorageBuffer& buffer);	//  ����Storage������
		//static void ResizeStorageBuffer(const std::string& storageBufferName, unsigned int size);	//  ����Storage��������С

		static std::shared_ptr<Shader> Create(const std::string name = "DefaultShader");
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& shaderSourcePath);
	private:
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformInt2(const std::string& name, const glm::ivec2& value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformInt3(const std::string& name, const glm::ivec3& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformInt4(const std::string& name, const glm::ivec4& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& value);

		std::string m_Name;	//  ��ɫ������
		unsigned int m_ShaderID;	//  ��ɫ��ID

		static std::unordered_map<std::string, ShaderUniformBuffer> s_UniformBuffers;	//  Uniform�������б�
		static std::unordered_map<std::string, ShaderStorageBuffer> s_StorageBuffers;	//  Storage�������б�
		std::unordered_map<std::string, GLuint> m_UniformLocations;	//  Uniform����λ���б�
		// TODO: ֮��������Ƴ�
		std::unordered_map<GLenum, std::string> m_ShaderSource;	//  ��ɫ��Դ����
	};
}