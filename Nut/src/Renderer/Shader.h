#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "Utils/ShaderUtils.h"

namespace Nut {

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

	struct ShaderResourceDeclaration {
		std::string Name;	//  ����
		unsigned int Location;	//  λ��
		// TODO: Dimension��ö��
		unsigned int Dimensions;	//  ά��
	};

	class Shader : public std::enable_shared_from_this<Shader>	//  ������ָ��
	{
	public:
		Shader(const std::string name = "DefaultShader");
		Shader(const std::string& name, const std::string& shaderSourcePath);	//  ���캯��

		void Reload(const std::string& shaderSourcePath);	//  ������ɫ��
		void ForceReload(const std::string& shaderSourcePath);	//  ǿ�����¼�����ɫ��

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
		static void SetUniformBuffer(const std::string& uniformBufferName, const void* data, unsigned int size);		//  ����Uniform������
		static void SetUniformBufferUniform(const std::string& uniformBufferName, const std::string& uniformName, const void* data);	//  ����Uniform������
		static void ClearUniformBuffers();	//  ���Uniform������

		// TODO: ֧��StorageBuffer
		//static void SetStorageBuffer(const std::string& storageBufferName, const ShaderStorageBuffer& buffer);	//  ����Storage������
		//static void ResizeStorageBuffer(const std::string& storageBufferName, unsigned int size);	//  ����Storage��������С

		static std::shared_ptr<Shader> Create(const std::string name = "DefaultShader");
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& shaderSourcePath);

		uint32_t GetUniformsLocation(const std::string name);	//  ��ȡUniform����λ��
		std::unordered_map<std::string, uint32_t> GetUniforms() const { return m_UniformsLocations; }
		std::unordered_map<std::string, ShaderResourceDeclaration> GetResourceDeclarations() const { return m_ResourceDeclarations; }
		std::string GetName() const { return m_Name; }	//  ��ȡ��ɫ������
		unsigned int GetShaderID() const { return m_ShaderID; }	//  ��ȡ��ɫ��ID
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
		unsigned int m_ShaderID = 0;	//  ��ɫ��ID
		std::string m_AssetPath;	//  ��Դ·��

		static std::unordered_map<std::string, ShaderUniformBuffer> s_UniformBuffers;	//  Uniform�������б�
		static std::unordered_map<std::string, ShaderStorageBuffer> s_StorageBuffers;	//  Storage�������б�
		std::unordered_map<std::string, uint32_t> m_UniformsLocations; //  Uniform�����б�
		std::unordered_map<std::string, ShaderResourceDeclaration> m_ResourceDeclarations; //  ��Դ�����б�
		// TODO: ֮��������Ƴ�
		std::unordered_map<GLenum, std::string> m_ShaderSource;	//  ��ɫ��Դ����
		std::unordered_map<GLenum, std::vector<uint32_t>> m_SPIRVData; // ��ɫ����SPIRV����

		friend class ShaderCompiler;	//  ��Ԫ��
	};
}