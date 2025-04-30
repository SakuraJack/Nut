#pragma once

#include "glad/glad.h"

namespace Nut {
	namespace ShaderUtils {

		inline static GLenum StageToGLShaderStage(const std::string_view stage)
		{
			if (stage == "Vertex")						return GL_VERTEX_SHADER;
			else if (stage == "Fragment")				return GL_FRAGMENT_SHADER;
			else if (stage == "Geometry")				return GL_GEOMETRY_SHADER;
			else if (stage == "Compute")				return GL_COMPUTE_SHADER;
			else if (stage == "TessControl")			return GL_TESS_CONTROL_SHADER;
			else if (stage == "TessEvaluation")			return GL_TESS_EVALUATION_SHADER;
			else
				NUT_WARN("Unknown shader stage: {0}", stage);
		}

		inline static std::string GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:				return "Vertex";
			case GL_FRAGMENT_SHADER:			return "Fragment";
			case GL_GEOMETRY_SHADER:			return "Geometry";
			case GL_COMPUTE_SHADER:			return "Compute";
			case GL_TESS_CONTROL_SHADER:		return "TessControl";
			case GL_TESS_EVALUATION_SHADER:	return "TessEvaluation";
			default:
				NUT_WARN("Unknown shader stage: {0}", stage);
				return "Unknown";
			}
		}
	}

	class ShaderCompiler
	{
		// TODO: 之后添加shaderc 
	public:
		ShaderCompiler(const std::string& shaderSourcePath);
		~ShaderCompiler();

		static std::unordered_map<GLenum, std::string> CompileShader(const std::string& shaderSourcePath);	//  编译着色器
		// TODO: 以后删除 

	private:
		void PreprocessShader(const std::string& shaderSourcePath);	//  预处理着色器

	private:
		std::filesystem::path m_ShaderSourcePath;
		std::unordered_map<GLenum, std::string> m_ShaderSource;	//  着色器源代码
	};
}