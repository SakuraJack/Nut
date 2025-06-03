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
		std::string Name; //  名称
		std::string BufferName; //  缓冲区名称
		ShaderUniformType Type;	//  类型
		uint64_t Size; //  大小
		uint64_t Offset; //  偏移量
	};

	struct ShaderUniformBuffer {
		std::string Name;	//  名称
		uint64_t Size;	//  大小
		uint64_t Binding;	//  绑定点
		RenderID BufferID;	//  缓冲区ID
		std::unordered_map<std::string, ShaderUniform> Uniforms;	//  Uniform变量列表
	};

	struct ShaderStorageBuffer {
		std::string Name;
		uint64_t Size;
		uint64_t Binding;
		RenderID BufferID;
		std::unordered_map<std::string, ShaderUniform> Uniforms;	//  Uniform变量列表
	};

	struct ShaderResourceDeclaration {
		std::string Name;	//  名称
		uint64_t Location;	//  位置
		TextureType Dimensions;	//  维度
	};

	class Shader : public std::enable_shared_from_this<Shader>	//  允许共享指针
	{
	public:
		struct ReflectionData
		{
			std::unordered_map<std::string, ShaderResourceDeclaration> Resources;
			// TODO: 考虑之后将UniformBuffer和StorageBuffer合并
			std::unordered_map<std::string, ShaderUniformBuffer> UniformBuffers;
			std::unordered_map<std::string, ShaderStorageBuffer> StorageBuffers;
			//  PlainUniform变量列表 TODO:之后可能会移除 兼容Vulkan的着色器规则
			std::unordered_map<std::string, uint32_t> m_UniformsLocations; 
		};

	public:
		Shader(const std::string name = "");
		Shader(const std::string& name, const std::string& shaderSourcePath);	//  构造函数
		~Shader();	//  析构函数

		void HotOverload();
		void Reload(const std::string& shaderSourcePath, bool forceCompile = false);
		void Rename(const std::string& name);	//  重命名着色器

		void Bind();		//  绑定着色器
		void Unbind();		//  解绑着色器

		static void PushUniformBuffer(RenderID bufferID, uint32_t offset, uint32_t size, void* data);
		static void PushStorageBuffer(RenderID bufferID, uint32_t offset, uint32_t size, void* data);

		static std::shared_ptr<Shader> Create(const std::string name = "DefaultShader");
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& shaderSourcePath);

		std::string GetName() const { return m_Name; }	//  获取着色器名称
		RenderID GetShaderID() const { return m_ShaderID; }	//  获取着色器ID
		std::unordered_map<std::string, ShaderUniformBuffer>& GetUniformBuffers() { return m_ReflectionData.UniformBuffers; }
		const std::unordered_map<std::string, ShaderUniformBuffer>& GetUniformBuffers() const { return m_ReflectionData.UniformBuffers; }
		std::unordered_map<std::string, ShaderStorageBuffer>& GetStorageBuffers() { return m_ReflectionData.StorageBuffers; }
		const std::unordered_map<std::string, ShaderStorageBuffer>& GetStorageBuffers() const { return m_ReflectionData.StorageBuffers; }
		const std::unordered_map<std::string, ShaderResourceDeclaration>& GetResourceDeclarations() const { return m_ReflectionData.Resources; }
		const std::unordered_map<std::string, uint32_t>& GetUniformLocations() const { return m_ReflectionData.m_UniformsLocations; }

	private:
		std::string m_Name;	//  着色器名称
		RenderID m_ShaderID = 0;	//  着色器ID
		std::string m_AssetPath;	//  资源路径

		ReflectionData m_ReflectionData;	//  反射数据
		std::unordered_map<GLenum, std::vector<uint32_t>> m_SPIRVData;	//  着色器二进制数据

		friend class ShaderCompiler;	//  友元类
	};
}