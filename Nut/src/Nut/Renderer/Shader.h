#pragma once

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "Nut/Utils/ShaderUtils.h"
#include "Image.h"

namespace Nut {

	class ShaderUniform
	{
	public:
		ShaderUniform() = default;
		ShaderUniform(const std::string& name, const std::string& buffername, ShaderUniformType type, uint64_t size, uint64_t offset)
			: Name(name), BufferName(buffername), Type(type), Size(size), Offset(offset) {}

		const std::string& GetName() const { return Name; }
		const std::string& GetBufferName() const { return BufferName; }
		ShaderUniformType GetType() const { return Type; }
		uint64_t GetSize() const { return Size; }
		uint64_t GetOffset() const { return Offset; }

	private:
		std::string Name; //  ����
		std::string BufferName; //  ����������
		ShaderUniformType Type;	//  ����
		uint64_t Size; //  ��С
		uint64_t Offset; //  ƫ����
	};

	struct ShaderUniformBuffer {
		std::string Name;	//  ����
		uint64_t Size;	//  ��С
		uint64_t Binding;	//  �󶨵�
		RenderID BufferID;	//  ������ID
		std::unordered_map<std::string, ShaderUniform> Uniforms;	//  Uniform�����б�
	};

	struct ShaderStorageBuffer {
		std::string Name;
		uint64_t Size;
		uint64_t Binding;
		RenderID BufferID;
		std::unordered_map<std::string, ShaderUniform> Uniforms;	//  Uniform�����б�
	};

	struct ShaderResourceDeclaration {
		std::string Name;	//  ����
		uint64_t Location;	//  λ��
		TextureType Dimensions;	//  ά��
	};

	class Shader : public std::enable_shared_from_this<Shader>	//  ������ָ��
	{
	public:
		struct ReflectionData
		{
			std::unordered_map<std::string, ShaderResourceDeclaration> Resources;
			// TODO: ����֮��UniformBuffer��StorageBuffer�ϲ�
			std::unordered_map<std::string, ShaderUniformBuffer> UniformBuffers;
			std::unordered_map<std::string, ShaderStorageBuffer> StorageBuffers;
			//  PlainUniform�����б� TODO:֮����ܻ��Ƴ� ����Vulkan����ɫ������
			std::unordered_map<std::string, uint32_t> m_UniformsLocations; 
		};

	public:
		Shader(const std::string name = "");
		Shader(const std::string& name, const std::string& shaderSourcePath);	//  ���캯��
		~Shader();	//  ��������

		void HotOverload();
		void Reload(const std::string& shaderSourcePath, bool forceCompile = false);
		void Rename(const std::string& name);	//  ��������ɫ��

		void Bind();		//  ����ɫ��
		void Unbind();		//  �����ɫ��

		static void PushUniformBuffer(RenderID bufferID, uint32_t offset, uint32_t size, void* data);
		static void PushStorageBuffer(RenderID bufferID, uint32_t offset, uint32_t size, void* data);

		static std::shared_ptr<Shader> Create(const std::string name = "DefaultShader");
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& shaderSourcePath);

		std::string GetName() const { return m_Name; }	//  ��ȡ��ɫ������
		RenderID GetShaderID() const { return m_ShaderID; }	//  ��ȡ��ɫ��ID
		std::unordered_map<std::string, ShaderUniformBuffer>& GetUniformBuffers() { return m_ReflectionData.UniformBuffers; }
		const std::unordered_map<std::string, ShaderUniformBuffer>& GetUniformBuffers() const { return m_ReflectionData.UniformBuffers; }
		std::unordered_map<std::string, ShaderStorageBuffer>& GetStorageBuffers() { return m_ReflectionData.StorageBuffers; }
		const std::unordered_map<std::string, ShaderStorageBuffer>& GetStorageBuffers() const { return m_ReflectionData.StorageBuffers; }
		const std::unordered_map<std::string, ShaderResourceDeclaration>& GetResourceDeclarations() const { return m_ReflectionData.Resources; }
		const std::unordered_map<std::string, uint32_t>& GetUniformLocations() const { return m_ReflectionData.m_UniformsLocations; }

	private:
		std::string m_Name;	//  ��ɫ������
		RenderID m_ShaderID = 0;	//  ��ɫ��ID
		std::string m_AssetPath;	//  ��Դ·��

		ReflectionData m_ReflectionData;	//  ��������
		std::unordered_map<GLenum, std::vector<uint32_t>> m_SPIRVData;	//  ��ɫ������������

		friend class ShaderCompiler;	//  ��Ԫ��
	};
}