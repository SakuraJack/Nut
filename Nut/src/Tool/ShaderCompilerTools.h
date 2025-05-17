#pragma once

#include "glad/glad.h"
#include <Renderer/Shader.h>

namespace Nut {

	class ShaderCompiler
	{
	public:
		ShaderCompiler(const std::string& shaderSourcePath);
		~ShaderCompiler();

		bool Compile();

	private:
		bool CompileOrGetBinaries(std::unordered_map<GLenum, std::vector<uint32_t>>& shaderBinaries, bool forceCompile = false);	//  ������ȡ�������ļ�
		void PreprocessShader();	//  Ԥ������ɫ��
		void TryGetCachedBinaries(const std::filesystem::path& cachePath, const std::string& extension, std::vector<uint32_t>& outputBinaries);
		void Reflect(std::vector<uint32_t>& data, std::unordered_map<std::string, ShaderUniformBuffer>& shaderUniformBuffers, std::string stage);

	private:
		bool m_UseOptimization = false;	//  �Ƿ�ʹ���Ż�
		std::filesystem::path m_ShaderSourcePath;
		std::unordered_map<GLenum, std::string> m_ShaderSource;	//  ��ɫ��Դ����
		std::unordered_map<GLenum, std::vector<uint32_t>> m_ShaderBinaries;	//  ��ɫ���������ļ�
		std::unordered_map<std::string, uint32_t> m_Uniforms;	//  �����Uniform�����б�
		std::unordered_map<std::string, ShaderResourceDeclaration> m_Resources;	//  �������Դ�б�

		friend class Shader;
	};
}