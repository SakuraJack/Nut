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
		std::unordered_map<GLenum, std::string> PreprocessShader(const std::string& inputSource);	//  Ԥ������ɫ��
		bool CompileOrGetBinaries(std::unordered_map<GLenum, std::vector<uint32_t>>& shaderBinaries, bool forceCompile = false);	//  ������ȡ�������ļ�
		void TryGetCachedBinaries(const std::filesystem::path& cachePath, const std::string& extension, std::vector<uint32_t>& outputBinaries);
		void Reflect(std::vector<uint32_t>& data, std::string stage);

	private:
		bool m_UseOptimization = false;	//  �Ƿ�ʹ���Ż�
		std::filesystem::path m_ShaderSourcePath;
		std::unordered_map<GLenum, std::string> m_ShaderSource;	//  ��ɫ��Դ����
		std::unordered_map<GLenum, std::vector<uint32_t>> m_SPIRVData;	//  ��ɫ���������ļ�
		Shader::ReflectionData m_ReflectionData;	//  ��������

		friend class Shader;
	};
}