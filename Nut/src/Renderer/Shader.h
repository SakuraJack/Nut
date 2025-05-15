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

		const std::string& GetName() const { return Name; }	//  获取名称
		ShaderUniformType GetType() const { return Type; }	//  获取类型
		unsigned int GetSize() const { return Size; }	//  获取大小
		unsigned int GetOffset() const { return Offset; }	//  获取偏移量

	private:
		std::string Name;	//  名称
		ShaderUniformType Type;	//  类型
		unsigned int Size;	//  大小
		unsigned int Offset;	//  偏移量
	};

	struct ShaderUniformBuffer {
		std::string Name;	//  名称
		unsigned int Size;	//  大小
		unsigned int Binding;	//  绑定点
		unsigned int BufferID;	//  缓冲区ID
		std::unordered_map<std::string, ShaderUniform> Uniforms;	//  Uniform变量列表
	};

	struct ShaderStorageBuffer {

	};

	struct ShaderResourceDeclaration {
		std::string Name;	//  名称
		unsigned int Location;	//  位置
		// TODO: Dimension改枚举
		unsigned int Dimensions;	//  维度
	};

	class Shader : public std::enable_shared_from_this<Shader>	//  允许共享指针
	{
	public:
		Shader(const std::string name = "DefaultShader");
		Shader(const std::string& name, const std::string& shaderSourcePath);	//  构造函数

		void Reload(const std::string& shaderSourcePath);	//  加载着色器
		void ForceReload(const std::string& shaderSourcePath);	//  强制重新加载着色器

		void Bind();		//  绑定着色器
		void Unbind();		//  解绑着色器

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

		static ShaderUniformBuffer& GetUniformBuffer(const std::string& uniformBufferName);	//  获取Uniform缓冲区
		static void SetUniformBuffer(const std::string& uniformBufferName, const void* data, unsigned int size);		//  设置Uniform缓冲区
		static void SetUniformBufferUniform(const std::string& uniformBufferName, const std::string& uniformName, const void* data);	//  设置Uniform缓冲区
		static void ClearUniformBuffers();	//  清除Uniform缓冲区

		// TODO: 支持StorageBuffer
		//static void SetStorageBuffer(const std::string& storageBufferName, const ShaderStorageBuffer& buffer);	//  设置Storage缓冲区
		//static void ResizeStorageBuffer(const std::string& storageBufferName, unsigned int size);	//  调整Storage缓冲区大小

		static std::shared_ptr<Shader> Create(const std::string name = "DefaultShader");
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& shaderSourcePath);

		uint32_t GetUniformsLocation(const std::string name);	//  获取Uniform变量位置
		std::unordered_map<std::string, uint32_t> GetUniforms() const { return m_UniformsLocations; }
		std::unordered_map<std::string, ShaderResourceDeclaration> GetResourceDeclarations() const { return m_ResourceDeclarations; }
		std::string GetName() const { return m_Name; }	//  获取着色器名称
		unsigned int GetShaderID() const { return m_ShaderID; }	//  获取着色器ID
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

		std::string m_Name;	//  着色器名称
		unsigned int m_ShaderID = 0;	//  着色器ID
		std::string m_AssetPath;	//  资源路径

		static std::unordered_map<std::string, ShaderUniformBuffer> s_UniformBuffers;	//  Uniform缓冲区列表
		static std::unordered_map<std::string, ShaderStorageBuffer> s_StorageBuffers;	//  Storage缓冲区列表
		std::unordered_map<std::string, uint32_t> m_UniformsLocations; //  Uniform变量列表
		std::unordered_map<std::string, ShaderResourceDeclaration> m_ResourceDeclarations; //  资源声明列表
		// TODO: 之后从这里移除
		std::unordered_map<GLenum, std::string> m_ShaderSource;	//  着色器源代码
		std::unordered_map<GLenum, std::vector<uint32_t>> m_SPIRVData; // 着色器的SPIRV数据

		friend class ShaderCompiler;	//  友元类
	};
}