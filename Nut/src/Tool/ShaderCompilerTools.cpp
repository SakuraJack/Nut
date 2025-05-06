#include "ntpch.h"
#include "ShaderCompilerTools.h"
#include "Utils/StringUtils.h"
#include "Utils/ShaderUtils.h"
#include "shaderc/shaderc.hpp"
#include "spirv_cross/spirv_cross.hpp"

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

bool Nut::ShaderCompiler::Compile(std::shared_ptr<Shader> shader, bool forceCompile)
{
	std::shared_ptr<ShaderCompiler> compiler = std::make_shared<ShaderCompiler>(shader->m_AssetPath);
	compiler->PreprocessShader();
	compiler->CompileOrGetBinaries(compiler->m_ShaderBinaries, forceCompile);	//  ǿ�Ʊ���

	if (shader->m_ShaderID) 
		glDeleteProgram(shader->m_ShaderID);	//  ɾ���ɵ���ɫ������
	shader->m_ShaderID = glCreateProgram();

	std::vector<GLuint> shaderIDs;

	for (auto& [stage, source] : compiler->m_ShaderBinaries) {
		GLuint shaderID = glCreateShader(stage);
		glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, source.data(), source.size() * sizeof(uint32_t));	//  ���ض������ļ�
		glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);	//  ר����ɫ��
		glAttachShader(shader->m_ShaderID, shaderID);	//  ������ɫ��
		shaderIDs.push_back(shaderID);	//  �����ɫ��ID
	}

	glLinkProgram(shader->m_ShaderID);	//  ������ɫ������
	GLuint linked = 0;
	glGetProgramiv(shader->m_ShaderID, GL_LINK_STATUS, (int*)&linked);	//  ��ȡ����״̬
	if (!linked) {
		GLint logLength = 0;
		glGetProgramiv(shader->m_ShaderID, GL_INFO_LOG_LENGTH, &logLength);	//  ��ȡ��־����
		std::vector<GLchar> log(logLength);
		glGetProgramInfoLog(shader->m_ShaderID, logLength, nullptr, log.data());	//  ��ȡ��־
		NUT_ERROR_TAG("Shader", "Shader linking failed: {0}", log.data());	//  ���������־
		glDeleteProgram(shader->m_ShaderID);	//  ɾ����ɫ������
		for (auto id : shaderIDs)
			glDeleteShader(id);	//  ɾ����ɫ��
		return false;
	}

	shader->m_ShaderSource.clear();
	shader->m_SPIRVData.clear();
	shader->m_ShaderSource = compiler->m_ShaderSource;	//  ������ɫ��Դ����
	shader->m_SPIRVData = compiler->m_ShaderBinaries;	//  ����SPIRV����

	for (auto& [stage, source] : compiler->m_ShaderBinaries) {
		compiler->Reflect(source, shader, Shader::s_UniformBuffers);		//  ����Uniform������
	}
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

void Nut::ShaderCompiler::Reflect(std::vector<uint32_t>& data, std::shared_ptr<Shader> shader, std::unordered_map<std::string, ShaderUniformBuffer>& shaderUniformBuffers)
{
	spirv_cross::Compiler compiler(data);
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	NUT_TRACE_TAG("Shader", "Shader reflect - {0}", m_ShaderSourcePath);
	NUT_TRACE_TAG("Shader", "Uniform Buffers: {0}", resources.uniform_buffers.size());
	NUT_TRACE_TAG("Shader", "Storage Buffers: {0}", resources.storage_buffers.size());
	NUT_TRACE_TAG("Shader", "Sampled Images: {0}", resources.sampled_images.size());
	NUT_TRACE_TAG("Shader", "Plain Uniforms: {0}", resources.gl_plain_uniforms.size());

	uint32_t bufferIndex = 0;
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

			glCreateBuffers(1, &buffer.BufferID);	//  ����Uniform������
			glNamedBufferData(buffer.BufferID, bufferSize, nullptr, GL_DYNAMIC_DRAW);	//  ���仺�����ڴ�
			// glBindBufferBase(GL_UNIFORM_BUFFER, buffer.Binding, buffer.BufferID);	//  ��Uniform������
		}
		else {
			ShaderUniformBuffer& buffer = shaderUniformBuffers[bufferName];
			if (bufferSize > buffer.Size) {
				buffer.Size = bufferSize;
				glDeleteBuffers(1, &buffer.BufferID);	//  ɾ���ɵ�Uniform������
				glCreateBuffers(1, &buffer.BufferID);	//  ����Uniform������
				glNamedBufferData(buffer.BufferID, bufferSize, nullptr, GL_DYNAMIC_DRAW);	//  ���仺�����ڴ�
			}
		}
		// TODO: ������
		//int sampler = 0; // ����������
		//for (const auto& resource : resources.sampled_images) {
		//	auto& type = compiler.get_type(resource.base_type_id);	//  ��ȡ����������
		//	auto bindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);	//  ��ȡ�󶨵�
		//	const auto& name = compiler.get_name(resource.id);	//  ��ȡ����
		//	uint32_t dimension = type.image.dim;

		//	GLuint location = glGetUniformLocation(shaderID, name.c_str());	//  ��ȡUniformλ��
		//	shaderResources[name] = { name, bindingPoint, 1 };	//  ����Shader��Դ����
		//	glUniform1i(location, sampler);	//  ����Uniformֵ
		//}
	}

	for (const auto& uniform : resources.gl_plain_uniforms) {
		std::string name = compiler.get_name(uniform.id);	//  ��ȡUniform����
		shader->m_Uniforms.push_back(name);	//  ���Uniform����
	}

}

void Nut::ShaderCompiler::CompileOrGetBinaries(std::unordered_map<GLenum, std::vector<uint32_t>>& shaderBinaries, bool forceCompile /*= false*/)
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
				return;
			}
			shaderBinaries[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
		}
	}
}
