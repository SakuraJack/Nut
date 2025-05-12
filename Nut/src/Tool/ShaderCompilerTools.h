#pragma once

#include "glad/glad.h"
#include <Renderer/Shader.h>

namespace Nut {

	class ShaderCompiler
	{
		// TODO: 之后添加shaderc 
	public:
		ShaderCompiler(const std::string& shaderSourcePath);
		~ShaderCompiler();

		bool Compile(uint32_t& shaderProgram);
		static bool Compile(std::shared_ptr<Shader> shader, bool forceCompile = false);

	private:
		void CompileOrGetBinaries(std::unordered_map<GLenum, std::vector<uint32_t>>& shaderBinaries, bool forceCompile = false);	//  编译或获取二进制文件
		void PreprocessShader();	//  预处理着色器
		void TryGetCachedBinaries(const std::filesystem::path& cachePath, const std::string& extension, std::vector<uint32_t>& outputBinaries);
		void Reflect(std::vector<uint32_t>& data, std::shared_ptr<Shader> shader, 
			std::unordered_map<std::string, ShaderUniformBuffer>& shaderUniformBuffers, std::string stage);
		void Reflect(std::vector<uint32_t>& data, uint32_t shaderID,
			std::unordered_map<std::string, uint32_t>& uniforms,
			std::unordered_map<std::string, ShaderResourceDeclaration>& shaderResources, 
			std::unordered_map<std::string, ShaderUniformBuffer>& shaderUniformBuffers, 
			std::string stage);
	private:
		bool m_UseOptimization = false;	//  是否使用优化
		std::filesystem::path m_ShaderSourcePath;
		std::unordered_map<GLenum, std::string> m_ShaderSource;	//  着色器源代码
		std::unordered_map<GLenum, std::vector<uint32_t>> m_ShaderBinaries;	//  着色器二进制文件
		std::unordered_map<std::string, uint32_t> m_Uniforms;	//  反射的Uniform变量列表
		std::unordered_map<std::string, ShaderResourceDeclaration> m_Resources;	//  反射的资源列表

		friend class Shader;
	};
}