#include "ntpch.h"
#include "ShaderCompilerTools.h"
#include "Utils/StringUtils.h"
#include "Utils/ShaderUtils.h"
#include "shaderc/shaderc.hpp"
#include "spirv_cross/spirv_cross.hpp"
#include "Renderer/Renderer.h"

namespace Nut {
	namespace Utils {
		static const char* GetShaderCachePath()
		{
			return "Resources/Cache/Shaders/";	//  ��ɫ������·��
		}
	}
}

Nut::ShaderCompiler::ShaderCompiler(const std::string& shaderSourcePath)
	: m_ShaderSourcePath(shaderSourcePath)
{

}

Nut::ShaderCompiler::~ShaderCompiler()
{

}

bool Nut::ShaderCompiler::Compile()
{
	PreprocessShader();
	bool succeed = CompileOrGetBinaries(m_ShaderBinaries, true);	//  ������ȡ�������ļ�
	if (!succeed) return false;
	for (auto& [stage, source] : m_ShaderBinaries) {
		Reflect(source, Shader::s_UniformBuffers, ShaderUtils::GLShaderStageToString(stage));		//  ����Uniform������
	}
	return true;
}

void Nut::ShaderCompiler::PreprocessShader()
{
	std::stringstream sourceStream;
	std::string fileString = Nut::Utils::String::ReadFileAndSkipBOM(m_ShaderSourcePath);	//  ��ȡ�ļ�������BOM
	Nut::Utils::String::CopyWithoutComments(fileString.begin(), fileString.end(), std::ostream_iterator<char>(sourceStream));
	std::string source = sourceStream.str();

	std::vector<std::pair<GLenum, size_t>> stagePositions;

	size_t startOfStage = 0;
	size_t pos = source.find_first_of('#');

	while (pos != std::string::npos) {
		const size_t endOfLine = source.find("\r\n", pos) + 1;
		std::vector<std::string> tokens = Nut::Utils::String::SplitStringAndKeepDelims(source.substr(pos, endOfLine - pos));
		size_t index = 1;
		if (tokens[1] == "type") {
			++index;

			const std::string_view stage = tokens[index];
			auto shaderStage = ShaderUtils::StageToGLShaderStage(stage);
			startOfStage = pos;
			stagePositions.push_back({ shaderStage, startOfStage });
		}

		pos = source.find_first_of('#', pos + 1);
	}

	for (int index = 0; index < stagePositions.size(); ++index) {
		if (index == stagePositions.size() - 1)	//  ���һ���׶�
		{
			auto startPos = stagePositions[index].second;
			std::string subSource = source.substr(startPos);
			unsigned int firstLineEnd = subSource.find_first_of("\r\n");
			std::string resSource = subSource.substr(firstLineEnd + 1);
			resSource = Nut::Utils::String::TrimWhitespace(resSource);
			m_ShaderSource[stagePositions[index].first] = resSource;
		}
		else if (index < stagePositions.size() - 1)
		{
			auto startPos = stagePositions[index].second;
			auto endPos = stagePositions[index + 1].second;
			std::string subSource = source.substr(startPos, endPos - startPos);
			unsigned int firstLineEnd = subSource.find_first_of("\r\n");
			std::string resSource = subSource.substr(firstLineEnd + 1);
			resSource = Nut::Utils::String::TrimWhitespace(resSource);
			m_ShaderSource[stagePositions[index].first] = resSource;
		}
	}
}

void Nut::ShaderCompiler::TryGetCachedBinaries(const std::filesystem::path& cachePath, const std::string& extension, std::vector<uint32_t>& outputBinaries)
{
	const auto path = cachePath / (m_ShaderSourcePath.filename().string() + extension);
	const std::string cacheFilePath = path.string();

	FILE* f = fopen(cacheFilePath.data(), "rb");
	if (!f) return;
	fseek(f, 0, SEEK_END);
	uint64_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	outputBinaries = std::vector<uint32_t>(size / sizeof(uint32_t));
	fread(outputBinaries.data(), sizeof(uint32_t), outputBinaries.size(), f);
	fclose(f);
}

void Nut::ShaderCompiler::Reflect(std::vector<uint32_t>& data, std::unordered_map<std::string, ShaderUniformBuffer>& shaderUniformBuffers, std::string stage)
{
	spirv_cross::Compiler compiler(data);
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	NUT_TRACE_TAG("Shader", "Shader Name - {0}, Shader Stage - {1}", m_ShaderSourcePath.filename(), stage);
	NUT_TRACE_TAG("Shader", "Uniform Buffers: {0}", resources.uniform_buffers.size());
	NUT_TRACE_TAG("Shader", "Storage Buffers: {0}", resources.storage_buffers.size());
	NUT_TRACE_TAG("Shader", "Sampled Images: {0}", resources.sampled_images.size());
	NUT_TRACE_TAG("Shader", "Plain Uniforms: {0}", resources.gl_plain_uniforms.size());

	uint32_t bufferIndex = 0;
	// Uniform������
	for (const auto& ubo : resources.uniform_buffers) {
		auto& bufferType = compiler.get_type(ubo.base_type_id);		//  ��ȡUniform����������
		int memberCount = bufferType.member_types.size(); //  ��ȡ��Ա����
		uint32_t bindingPoint = compiler.get_decoration(ubo.id, spv::DecorationBinding);	//  ��ȡ�󶨵�
		uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);	//  ��ȡ��������С
		std::string bufferName = ubo.name;

		if (shaderUniformBuffers.find(bufferName) == shaderUniformBuffers.end()) {
			ShaderUniformBuffer& buffer = shaderUniformBuffers[bufferName];
			buffer.Name = bufferName;
			buffer.Binding = bindingPoint;
			buffer.Size = bufferSize;
			buffer.Uniforms.reserve(memberCount);
			for (int i = 0; i < memberCount; ++i) {
				auto type = compiler.get_type(bufferType.member_types[i]);
				const auto& memberName = compiler.get_member_name(bufferType.self, i);
				auto size = compiler.get_declared_struct_member_size(bufferType, i);
				auto offset = compiler.type_struct_member_offset(bufferType, i);

				ShaderUniform uniform(memberName, ShaderUtils::SPIRTypeToShaderUniformType(type), size, offset);	//  ����Uniform����
				buffer.Uniforms.insert({ memberName, uniform });	//  ����Uniform����
			}

			Renderer::Submit([&buffer, bufferSize]() {	//  �ύ��Ⱦ��
				glCreateBuffers(1, &buffer.BufferID);	//  ����Uniform������
				glNamedBufferData(buffer.BufferID, bufferSize, nullptr, GL_DYNAMIC_DRAW);	//  ���仺�����ڴ�
				glBindBufferBase(GL_UNIFORM_BUFFER, buffer.Binding, buffer.BufferID);	//  ��Uniform������
				});
		}
		else {
			ShaderUniformBuffer& buffer = shaderUniformBuffers[bufferName];
			if (bufferSize > buffer.Size) {
				buffer.Size = bufferSize;
				Renderer::Submit([&buffer]() {	//  �ύ��Ⱦ��
					glDeleteBuffers(1, &buffer.BufferID);	//  ɾ���ɵ�Uniform������
					glCreateBuffers(1, &buffer.BufferID);	//  ����Uniform������
					glNamedBufferData(buffer.BufferID, buffer.Size, nullptr, GL_DYNAMIC_DRAW);	//  ���仺�����ڴ�
					glBindBufferBase(GL_UNIFORM_BUFFER, buffer.Binding, buffer.BufferID);	//  ��Uniform������
					});
			}
		}
	}

	// ������
	int sampler = resources.sampled_images.size();
	for (const auto& resource : resources.sampled_images) {
		auto& type = compiler.get_type(resource.base_type_id);
		auto bindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);
		const auto& name = compiler.get_name(resource.id);
		uint32_t dimension = type.image.dim;
		m_Resources[name] = { name, bindingPoint, dimension };

	}

	// ��¶Uniform
	for (const auto& uniform : resources.gl_plain_uniforms) {
		std::string name = compiler.get_name(uniform.id);
		uint32_t location = compiler.get_decoration(uniform.id, spv::DecorationLocation);
		m_Uniforms[name] = location;
	}
}

bool Nut::ShaderCompiler::CompileOrGetBinaries(std::unordered_map<GLenum, std::vector<uint32_t>>& shaderBinaries, bool forceCompile /*= false*/)
{
	const std::filesystem::path shaderCachePath = Utils::GetShaderCachePath();
	for (auto& [stage, source] : m_ShaderSource) {
		const std::string stageExtension = ShaderUtils::GLShaderStageCachedFileExtension(stage);

		if (!forceCompile && stage) {
			TryGetCachedBinaries(shaderCachePath, stageExtension, shaderBinaries[stage]);
		}

		if (shaderBinaries[stage].empty()) {
			const std::string& shaderSource = source;
			shaderc::Compiler compiler;
			shaderc::CompileOptions ops;
			ops.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
			ops.AddMacroDefinition("OPENGL");
			const bool optimize = m_UseOptimization;
			if (optimize) {
				ops.SetOptimizationLevel(shaderc_optimization_level_performance);
			}

			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderSource, ShaderUtils::GLShaderStageToShaderc(stage), m_ShaderSourcePath.string().c_str(), ops);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				NUT_ERROR_TAG("Shader", "Shader compilation failed: {0}", module.GetErrorMessage());
				return false;
			}
			shaderBinaries[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
		}
	}
}
