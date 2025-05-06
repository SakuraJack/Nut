#pragma once

#include "glad/glad.h"
#include <Renderer/Shader.h>

namespace Nut {

	class ShaderCompiler
	{
		// TODO: ֮�����shaderc 
	public:
		ShaderCompiler(const std::string& shaderSourcePath);
		~ShaderCompiler();

		static bool Compile(std::shared_ptr<Shader> shader, bool forceCompile = false);

	private:
		void CompileOrGetBinaries(std::unordered_map<GLenum, std::vector<uint32_t>>& shaderBinaries, bool forceCompile = false);	//  ������ȡ�������ļ�
		void PreprocessShader();	//  Ԥ������ɫ��
		void TryGetCachedBinaries(const std::filesystem::path& cachePath, const std::string& extension, std::vector<uint32_t>& outputBinaries);
		void Reflect(std::vector<uint32_t>& data, std::shared_ptr<Shader> shader, std::unordered_map<std::string, ShaderUniformBuffer>& shaderUniformBuffers);
	private:
		std::filesystem::path m_ShaderSourcePath;
		std::unordered_map<GLenum, std::string> m_ShaderSource;	//  ��ɫ��Դ����
		std::unordered_map<GLenum, std::vector<uint32_t>> m_ShaderBinaries;	//  ��ɫ���������ļ�
		bool m_UseOptimization = false;	//  �Ƿ�ʹ���Ż�

		friend class Shader;
	};
}