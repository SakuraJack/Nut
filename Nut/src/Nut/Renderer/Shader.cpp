#include "ntpch.h"
#include "Shader.h"
#include "Nut/Tool/ShaderCompilerTools.h"
#include "Renderer.h"

Nut::Shader::Shader(const std::string name /*= ""*/)
	: m_Name(name), m_AssetPath("resources/Shaders/" + name + ".glsl")
{
	ShaderCompiler compiler(m_AssetPath);
	if (!compiler.Reload(true)) {
		NUT_CORE_ERROR_TAG("Shader", "Failed to compile shader: {0}", name);
	}
	else {
		m_ReflectionData = std::move(compiler.m_ReflectionData);
		m_SPIRVData = std::move(compiler.m_SPIRVData);
		HotOverload();
	}
}

Nut::Shader::Shader(const std::string& name, const std::string& shaderSourcePath)
	: m_Name(name), m_AssetPath(shaderSourcePath)
{
	ShaderCompiler compiler(m_AssetPath);
	if (!compiler.Reload(true)) {
		NUT_CORE_ERROR_TAG("Shader", "Failed to compile shader: {0}", name);
	}
	else {
		m_ReflectionData = std::move(compiler.m_ReflectionData);
		m_SPIRVData = std::move(compiler.m_SPIRVData);
		HotOverload();
	}
}

Nut::Shader::~Shader()
{
	RenderID shaderID = m_ShaderID;
	Renderer::Submit([shaderID]() {
		glDeleteProgram(shaderID);
		});

	// �ͷ���Դ
	for (auto& [name, buffer] : m_ReflectionData.UniformBuffers) {
		if (buffer.BufferID) {
			RenderID bufferID = buffer.BufferID;
			Renderer::Submit([bufferID]() {
				glDeleteBuffers(1, &bufferID);
				});
		}
	}

	for (auto& [name, buffer] : m_ReflectionData.StorageBuffers) {
		if (buffer.BufferID) {
			RenderID bufferID = buffer.BufferID;
			Renderer::Submit([bufferID]() {
				glDeleteBuffers(1, &bufferID);
				});
		}
	}
}


void Nut::Shader::HotOverload()
{
	Renderer::Submit([this]() {
		GLuint oldShaderID = m_ShaderID;
		GLuint newProgram = glCreateProgram();
		std::vector<GLuint> shaderIDs;

		for (auto& [stage, source] : m_SPIRVData) {
			GLuint shaderID = glCreateShader(stage);
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, source.data(), source.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(newProgram, shaderID);
			shaderIDs.push_back(shaderID);
		}

		glLinkProgram(newProgram);
		GLint linked = 0;
		glGetProgramiv(newProgram, GL_LINK_STATUS, &linked);

		for (auto id : shaderIDs)
			glDeleteShader(id);

		if (!linked) {
			GLint logLength = 0;
			glGetProgramiv(newProgram, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<GLchar> log(logLength);
			glGetProgramInfoLog(newProgram, logLength, nullptr, log.data());
			NUT_CORE_ERROR_TAG("Shader", "Shader linking failed: {0}", log.data());
			glDeleteProgram(newProgram); // ������ program
			return; // ���־ɵ� m_ShaderID ����
		}

		if (oldShaderID)
			glDeleteProgram(oldShaderID); // ɾ���ɵ� program
		m_ShaderID = newProgram;
		});
}

void Nut::Shader::Reload(const std::string& shaderSourcePath, bool forceCompile)
{
	m_AssetPath = shaderSourcePath;
	ShaderCompiler compiler(m_AssetPath);
	if (!compiler.Reload(forceCompile)) {
		NUT_CORE_ERROR_TAG("Shader", "Failed to compile shader: {0}", m_Name);
	}
	else {
		m_ReflectionData = std::move(compiler.m_ReflectionData);
		HotOverload();
	}
}

void Nut::Shader::Rename(const std::string& name)
{
	m_Name = name;
}

void Nut::Shader::Bind()
{
	Renderer::Submit([this]() {
		glUseProgram(m_ShaderID);
		});
}

void Nut::Shader::Unbind()
{
	Renderer::Submit([this]() {
		glUseProgram(0);
		});
}

void Nut::Shader::PushUniformBuffer(RenderID bufferID, uint32_t offset, uint32_t size, void* data)
{
	Renderer::Submit([bufferID, offset, size, data]() {
		glNamedBufferSubData(bufferID, offset, size, data);
		});
}

void Nut::Shader::PushStorageBuffer(RenderID bufferID, uint32_t offset, uint32_t size, void* data)
{
	Renderer::Submit([bufferID, offset, size, data]() {
		glNamedBufferSubData(bufferID, offset, size, data);
		});
}

std::shared_ptr<Nut::Shader> Nut::Shader::Create(const std::string name /*= "DefaultShader"*/)
{
	return std::make_shared<Nut::Shader>(name);
}

std::shared_ptr<Nut::Shader> Nut::Shader::Create(const std::string& name, const std::string& shaderSourcePath)
{
	return std::make_shared<Nut::Shader>(name, shaderSourcePath);
}

Nut::ShaderLibrary::ShaderLibrary()
{
}

Nut::ShaderLibrary::~ShaderLibrary()
{
}

void Nut::ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
{
	auto name = shader->GetName();
	m_Shaders[name] = shader;
}

void Nut::ShaderLibrary::Load(std::string_view name, bool forceCompile, bool disableOptimization)
{
	auto shader = Shader::Create(name.data());
	Add(shader);
}

void Nut::ShaderLibrary::Load(std::string_view name, const std::string& path)
{
	auto shader = Shader::Create(name.data(), path);
	Add(shader);
}

const std::shared_ptr<Nut::Shader>& Nut::ShaderLibrary::Get(const std::string& name) const
{
	auto it = m_Shaders.find(name);
	if (it != m_Shaders.end())
		return it->second;
	return nullptr;
}


