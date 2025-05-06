#pragma once
#include "glad/glad.h"
#include "shaderc/shaderc.hpp"
#include "spirv_cross/spirv_cross.hpp"

namespace Nut {
	enum class ShaderUniformType
	{
		None = 0, Bool, UInt, Int, Float, Vec2, Vec3, Vec4, Mat3, Mat4,
	};

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
			case GL_COMPUTE_SHADER:				return "Compute";
			case GL_TESS_CONTROL_SHADER:		return "TessControl";
			case GL_TESS_EVALUATION_SHADER:		return "TessEvaluation";
			default:
				NUT_WARN("Unknown shader stage: {0}", stage);
				return "Unknown";
			}
		}

		inline static std::string GLShaderStageCachedFileExtension(const uint32_t stage) {
			switch (stage)
			{
			case GL_VERTEX_SHADER:				return ".cached_vert";
			case GL_FRAGMENT_SHADER:			return ".cached_frag";
			case GL_GEOMETRY_SHADER:			return ".cached_geom";
			case GL_COMPUTE_SHADER:				return ".cached_comp";
			case GL_TESS_CONTROL_SHADER:		return ".cached_tesc";
			case GL_TESS_EVALUATION_SHADER:		return ".cached_tese";
			default:
				NUT_WARN("Unknown shader stage: {0}", stage);
				return "Unknown";
			}
		}

		inline static shaderc_shader_kind GLShaderStageToShaderc(const uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:			return shaderc_vertex_shader;
			case GL_FRAGMENT_SHADER:		return shaderc_fragment_shader;
			case GL_COMPUTE_SHADER:			return shaderc_compute_shader;
			case GL_GEOMETRY_SHADER:		return shaderc_geometry_shader;
			case GL_TESS_CONTROL_SHADER:	return shaderc_tess_control_shader;
			case GL_TESS_EVALUATION_SHADER:	return shaderc_tess_evaluation_shader;
			}
			return {};
		}

		inline static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
		{
			switch (type.basetype)
			{
			case spirv_cross::SPIRType::Boolean:		return ShaderUniformType::Bool;
			case spirv_cross::SPIRType::Int:			return ShaderUniformType::Int;
			case spirv_cross::SPIRType::UInt:			return ShaderUniformType::UInt;
			case spirv_cross::SPIRType::Float:
				if (type.vecsize == 1)					return ShaderUniformType::Float;
				if (type.vecsize == 2)					return ShaderUniformType::Vec2;
				if (type.vecsize == 3)					return ShaderUniformType::Vec3;
				if (type.vecsize == 4)					return ShaderUniformType::Vec4;
				if (type.columns == 3)					return ShaderUniformType::Mat3;
				if (type.columns == 4)					return ShaderUniformType::Mat4;
				break;
			}
			return ShaderUniformType::None;
		}
	}
}