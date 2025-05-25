#include "ntpch.h"
#include "ShaderCompilerTools.h"
#include "Utils/StringUtils.h"
#include "Utils/ShaderUtils.h"
#include "Utils/ImageUtils.h"
#include "shaderc/shaderc.hpp"
#include "spirv_cross/spirv_cross.hpp"
#include "Renderer/Renderer.h"

namespace Nut {
	namespace Utils {
		static const char* GetShaderCachePath()
		{
			return "Resources/Cache/Shaders/";	//  着色器缓存路径
		}

		static void CreateCacheDirectory()
		{
			std::string cacheDirectory = GetShaderCachePath();
			if (!std::filesystem::exists(cacheDirectory)) {
				std::filesystem::create_directories(cacheDirectory);	//  创建缓存目录
			}
		}
	}
}

Nut::ShaderCompiler::ShaderCompiler(const std::string& shaderSourcePath, bool useOptimization)
	: m_ShaderSourcePath(shaderSourcePath), m_UseOptimization(useOptimization)
{

}

Nut::ShaderCompiler::~ShaderCompiler()
{

}

bool Nut::ShaderCompiler::Reload(bool forceCompile)
{
	m_ShaderSource.clear();
	m_SPIRVData.clear();

	Utils::CreateCacheDirectory();
	const std::string source = Nut::Utils::String::ReadFileAndSkipBOM(m_ShaderSourcePath);
	if (source.empty()) {
		NUT_ERROR_TAG("Shader", "加载着色器文件失败: {0}", m_ShaderSourcePath);
		return false;
	}

	NUT_TRACE_TAG("Shader", "加载着色器文件: {0}", m_ShaderSourcePath);
	m_ShaderSource = PreprocessShader(source);	//  预处理着色器
	bool compileSuccess = CompileOrGetBinaries(m_SPIRVData, forceCompile);	//  编译或获取二进制文件
	if (!compileSuccess) {
		NUT_ERROR_TAG("Shader", "编译着色器失败: {0}", m_ShaderSourcePath);
		return false;
	}

	for (auto& [stage, source] : m_SPIRVData) {
		const std::string stageExtension = ShaderUtils::GLShaderStageToString(stage);
		Reflect(source, stageExtension);
	}

	return true;
}

std::unordered_map<GLenum, std::string> Nut::ShaderCompiler::PreprocessShader(const std::string& inputSource)
{
	std::stringstream sourceStream;
	Nut::Utils::String::CopyWithoutComments(inputSource.begin(), inputSource.end(), std::ostream_iterator<char>(sourceStream));
	std::string source = sourceStream.str();

	std::vector<std::pair<GLenum, size_t>> stagePositions;
	std::unordered_map<GLenum, std::string> shaderSources;

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
		if (index == stagePositions.size() - 1)	//  最后一个阶段
		{
			auto startPos = stagePositions[index].second;
			std::string subSource = source.substr(startPos);
			unsigned int firstLineEnd = subSource.find_first_of("\r\n");
			std::string resSource = subSource.substr(firstLineEnd + 1);
			resSource = Nut::Utils::String::TrimWhitespace(resSource);
			shaderSources[stagePositions[index].first] = resSource;
		}
		else if (index < stagePositions.size() - 1)
		{
			auto startPos = stagePositions[index].second;
			auto endPos = stagePositions[index + 1].second;
			std::string subSource = source.substr(startPos, endPos - startPos);
			unsigned int firstLineEnd = subSource.find_first_of("\r\n");
			std::string resSource = subSource.substr(firstLineEnd + 1);
			resSource = Nut::Utils::String::TrimWhitespace(resSource);
			shaderSources[stagePositions[index].first] = resSource;
		}
	}
	return shaderSources;
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

void Nut::ShaderCompiler::Reflect(std::vector<uint32_t>& data, std::string stage)
{
	spirv_cross::Compiler compiler(data);
	spirv_cross::ShaderResources resources = compiler.get_shader_resources();

	NUT_TRACE_TAG("Shader", "Shader Name - {0}, Shader Stage - {1}", m_ShaderSourcePath.filename(), stage);
	NUT_TRACE_TAG("Shader", "Uniform Buffers: {0}", resources.uniform_buffers.size());
	NUT_TRACE_TAG("Shader", "Storage Buffers: {0}", resources.storage_buffers.size());
	NUT_TRACE_TAG("Shader", "Sampled Images: {0}", resources.sampled_images.size());
	NUT_TRACE_TAG("Shader", "Plain Uniforms: {0}", resources.gl_plain_uniforms.size());

	uint32_t bufferIndex = 0;
	// Uniform缓冲区
	for (const auto& ubo : resources.uniform_buffers) {
		auto& bufferType = compiler.get_type(ubo.base_type_id);		//  获取Uniform缓冲区类型
		int memberCount = bufferType.member_types.size(); //  获取成员数量
		uint32_t bindingPoint = compiler.get_decoration(ubo.id, spv::DecorationBinding);	//  获取绑定点
		uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);	//  获取缓冲区大小
		std::string bufferName = ubo.name;

		if (m_ReflectionData.UniformBuffers.find(bufferName) == m_ReflectionData.UniformBuffers.end()) {
			ShaderUniformBuffer& buffer = m_ReflectionData.UniformBuffers[bufferName];
			buffer.Name = bufferName;
			buffer.Binding = bindingPoint;
			buffer.Size = bufferSize;
			buffer.Uniforms.reserve(memberCount);
			for (int i = 0; i < memberCount; ++i) {
				auto type = compiler.get_type(bufferType.member_types[i]);
				const auto& memberName = compiler.get_member_name(bufferType.self, i);
				auto size = compiler.get_declared_struct_member_size(bufferType, i);
				auto offset = compiler.type_struct_member_offset(bufferType, i);

				ShaderUniform uniform(memberName, bufferName, ShaderUtils::SPIRTypeToShaderUniformType(type), size, offset);	//  创建Uniform变量
				buffer.Uniforms.insert({ memberName, uniform });	//  插入Uniform变量
			}

			Renderer::Submit([&buffer, bufferSize]() {
				glCreateBuffers(1, &buffer.BufferID);	//  创建Uniform缓冲区
				glNamedBufferData(buffer.BufferID, bufferSize, nullptr, GL_DYNAMIC_DRAW);	//  分配缓冲区内存
				glBindBufferBase(GL_UNIFORM_BUFFER, buffer.Binding, buffer.BufferID);	//  绑定Uniform缓冲区
				});
		}
		else {
			ShaderUniformBuffer& buffer = m_ReflectionData.UniformBuffers[bufferName];
			if (bufferSize > buffer.Size) {
				buffer.Size = bufferSize;
				Renderer::Submit([&buffer]() {
					glDeleteBuffers(1, &buffer.BufferID);	//  删除旧的Uniform缓冲区
					glCreateBuffers(1, &buffer.BufferID);	//  创建Uniform缓冲区
					glNamedBufferData(buffer.BufferID, buffer.Size, nullptr, GL_DYNAMIC_DRAW);	//  分配缓冲区内存
					glBindBufferBase(GL_UNIFORM_BUFFER, buffer.Binding, buffer.BufferID);	//  绑定Uniform缓冲区
					});
			}
		}
	}

	// Storage缓冲区
	for (const auto& ssbo : resources.storage_buffers) {
		auto& bufferType = compiler.get_type(ssbo.base_type_id);
		int memberCount = bufferType.member_types.size();
		uint32_t bindingPoint = compiler.get_decoration(ssbo.id, spv::DecorationBinding);
		uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
		std::string bufferName = ssbo.name;

		if (m_ReflectionData.StorageBuffers.find(bufferName) == m_ReflectionData.StorageBuffers.end()) {
			ShaderStorageBuffer& buffer = m_ReflectionData.StorageBuffers[bufferName];
			buffer.Name = bufferName;
			buffer.Binding = bindingPoint;
			buffer.Size = bufferSize;
			buffer.Uniforms.reserve(memberCount);
			for (int i = 0; i < memberCount; ++i) {
				auto type = compiler.get_type(bufferType.member_types[i]);
				const auto& memberName = compiler.get_member_name(bufferType.self, i);
				auto size = compiler.get_declared_struct_member_size(bufferType, i);
				auto offset = compiler.type_struct_member_offset(bufferType, i);

				ShaderUniform uniform(memberName, bufferName, ShaderUtils::SPIRTypeToShaderUniformType(type), size, offset);
				buffer.Uniforms.insert({ memberName, uniform });
			}

			Renderer::Submit([&buffer, bufferSize]() {	
				glCreateBuffers(1, &buffer.BufferID);	
				glNamedBufferData(buffer.BufferID, bufferSize, nullptr, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer.Binding, buffer.BufferID);
				});
		}
		else {
			ShaderStorageBuffer& buffer = m_ReflectionData.StorageBuffers[bufferName];
			if (bufferSize > buffer.Size) {
				buffer.Size = bufferSize;
				Renderer::Submit([&buffer]() {
					glDeleteBuffers(1, &buffer.BufferID);
					glCreateBuffers(1, &buffer.BufferID);
					glNamedBufferData(buffer.BufferID, buffer.Size, nullptr, GL_DYNAMIC_DRAW);
					glBindBufferBase(GL_SHADER_STORAGE_BUFFER, buffer.Binding, buffer.BufferID);
					});
			}
		}
	}

	// 采样器
	int sampler = resources.sampled_images.size();
	for (const auto& resource : resources.sampled_images) {
		auto& type = compiler.get_type(resource.base_type_id);
		auto bindingPoint = compiler.get_decoration(resource.id, spv::DecorationBinding);
		const auto& name = compiler.get_name(resource.id);
		TextureType dimension = Nut::ImageUtils::SPVDimensionToTextureType(type.image.dim);
		m_ReflectionData.Resources[name] = { name, bindingPoint, dimension };
	}

	// 裸露Uniform TODO: 之后可能会移除
	for (const auto& uniform : resources.gl_plain_uniforms) {
		std::string name = compiler.get_name(uniform.id);
		uint32_t location = compiler.get_decoration(uniform.id, spv::DecorationLocation);
		m_ReflectionData.m_UniformsLocations[name] = location;
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
			// 编译计算着色器禁用优化(shaderc内部问题)
			const bool optimize = m_UseOptimization && stage != GL_COMPUTE_SHADER;
			if (optimize) {
				ops.SetOptimizationLevel(shaderc_optimization_level_performance);
			}

			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderSource, ShaderUtils::GLShaderStageToShaderc(stage), m_ShaderSourcePath.string().c_str(), ops);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				NUT_ERROR_TAG("Shader", "着色器编译失败: {0}", module.GetErrorMessage());
				return false;
			}
			shaderBinaries[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
		}
	}
}
