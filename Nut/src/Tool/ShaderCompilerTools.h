#pragma once

#include "glad/glad.h"
#include <Renderer/Shader.h>

namespace Nut {

	class ShaderCompiler
	{
	public:
		ShaderCompiler(const std::string& shaderSourcePath, bool useOptimization = false);
		~ShaderCompiler();

		bool Reload(bool forceCompile = false);

	private:
		std::unordered_map<GLenum, std::string> PreprocessShader(const std::string& inputSource);	//  预处理着色器
		bool CompileOrGetBinaries(std::unordered_map<GLenum, std::vector<uint32_t>>& shaderBinaries, bool forceCompile = false);	//  编译或获取二进制文件
		void TryGetCachedBinaries(const std::filesystem::path& cachePath, const std::string& extension, std::vector<uint32_t>& outputBinaries);
		void Reflect(std::vector<uint32_t>& data, std::string stage);

	private:
		bool m_UseOptimization = false;	//  是否使用优化
		std::filesystem::path m_ShaderSourcePath;
		std::unordered_map<GLenum, std::string> m_ShaderSource;	//  着色器源代码
		std::unordered_map<GLenum, std::vector<uint32_t>> m_SPIRVData;	//  着色器二进制文件
		Shader::ReflectionData m_ReflectionData;	//  反射数据

		friend class Shader;
	};
}