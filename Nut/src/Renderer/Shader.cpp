#include "ntpch.h"
#include "Shader.h"
#include "Core/Log.h"
#include "Tool/ShaderCompilerTools.h"
#include "Renderer.h"

Nut::Shader::Shader(const std::string name /*= "DefaultShader"*/)
	: m_Name(name), m_AssetPath("Resources/Shaders/" + name + ".glsl")
{
	ShaderCompiler compiler(m_AssetPath);
	if (!compiler.Compile()) {
		NUT_ERROR_TAG("Shader", "Failed to compile shader: {0}", name);
	}
	else {
		m_ShaderSource = std::move(compiler.m_ShaderSource);
		m_SPIRVData = std::move(compiler.m_ShaderBinaries);
		m_UniformsLocations = std::move(compiler.m_Uniforms);
		m_ResourceDeclarations = std::move(compiler.m_Resources);
		HotOverload();
	}
}

Nut::Shader::~Shader()
{
	RenderID shaderID = m_ShaderID;
	Renderer::Submit([shaderID]() {
		glDeleteProgram(shaderID);
		});
	/*for (auto& [name, buffer] : s_UniformBuffers)
	{
		glDeleteBuffers(1, &buffer.BufferID);
	}*/
	m_ShaderSource.clear();
	m_SPIRVData.clear();
	m_UniformsLocations.clear();
	m_ResourceDeclarations.clear();
}

Nut::Shader::Shader(const std::string& name, const std::string& shaderSourcePath)
	: m_Name(name), m_AssetPath(shaderSourcePath)
{
	ShaderCompiler compiler(m_AssetPath);
	if (!compiler.Compile()) {
		NUT_ERROR_TAG("Shader", "Failed to compile shader: {0}", name);
	}
	else {
		m_ShaderSource = std::move(compiler.m_ShaderSource);
		m_SPIRVData = std::move(compiler.m_ShaderBinaries);
		m_UniformsLocations = std::move(compiler.m_Uniforms);
		m_ResourceDeclarations = std::move(compiler.m_Resources);
		HotOverload();
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
			NUT_ERROR_TAG("Shader", "Shader linking failed: {0}", log.data());
			glDeleteProgram(newProgram); // 清理新 program
			return; // 保持旧的 m_ShaderID 不变
		}

		if (oldShaderID)
			glDeleteProgram(oldShaderID); // 删除旧的 program
		m_ShaderID = newProgram;
		});
}

void Nut::Shader::Reload(const std::string& shaderSourcePath)
{
	m_AssetPath = shaderSourcePath;
	ShaderCompiler compiler(m_AssetPath);
	if (!compiler.Compile()) {
		NUT_ERROR_TAG("Shader", "Failed to compile shader: {0}", m_Name);
	}
	else {
		m_ShaderSource = std::move(compiler.m_ShaderSource);
		m_SPIRVData = std::move(compiler.m_ShaderBinaries);
		m_UniformsLocations = std::move(compiler.m_Uniforms);
		m_ResourceDeclarations = std::move(compiler.m_Resources);
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

void Nut::Shader::SetUniform(const std::string& name, int value)
{
	UploadUniformInt(name, value);
}

void Nut::Shader::SetUniform(const std::string& name, float value)
{
	UploadUniformFloat(name, value);
}

void Nut::Shader::SetUniform(const std::string& name, const glm::ivec2 value)
{
	UploadUniformInt2(name, value);
}

void Nut::Shader::SetUniform(const std::string& name, const glm::vec2 value)
{
	UploadUniformFloat2(name, value);
}

void Nut::Shader::SetUniform(const std::string& name, const glm::ivec3 value)
{
	UploadUniformInt3(name, value);
}

void Nut::Shader::SetUniform(const std::string& name, const glm::vec3 value)
{
	UploadUniformFloat3(name, value);
}

void Nut::Shader::SetUniform(const std::string& name, const glm::ivec4 value)
{
	UploadUniformInt4(name, value);
}

void Nut::Shader::SetUniform(const std::string& name, const glm::vec4 value)
{
	UploadUniformFloat4(name, value);
}

void Nut::Shader::SetUniform(const std::string& name, const glm::mat3 value)
{
	UploadUniformMat3(name, value);
}

void Nut::Shader::SetUniform(const std::string& name, const glm::mat4 value)
{
	UploadUniformMat4(name, value);
}

Nut::ShaderUniformBuffer& Nut::Shader::GetUniformBuffer(const std::string& uniformBufferName)
{
	auto it = s_UniformBuffers.find(uniformBufferName);
	if (it == s_UniformBuffers.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform buffer not found: {0}", uniformBufferName);
		return s_UniformBuffers[""];
	}
	return it->second;
}

void Nut::Shader::SetUniformBuffer(const std::string& uniformBufferName, const void* data, uint64_t size)
{
	ShaderUniformBuffer* buffer = nullptr;
	for (auto& [name, uniformBuffer] : s_UniformBuffers) {
		if (name == uniformBufferName) {
			buffer = &uniformBuffer;
			break;
		}
	}
	if (!buffer) return;
	Renderer::Submit([buffer, data]() {
		glNamedBufferSubData(buffer->BufferID, 0, buffer->Size, data);
		});
}

void Nut::Shader::SetUniformBufferUniform(const std::string& uniformBufferName, const std::string& uniformName, const void* data)
{
	ShaderUniformBuffer* buffer = nullptr;
	ShaderUniform* uniform = nullptr;
	for (auto& [bufferName, uniformBuffer] : s_UniformBuffers) {
		if (bufferName == uniformBufferName) {
			buffer = &uniformBuffer;
			for (auto& [name, uniformData] : uniformBuffer.Uniforms) {
				if (name == uniformName) {
					uniform = &uniformData;
					break;
				}
			}
		}
	}
	if (!buffer || !uniform) return;

	Renderer::Submit([uniform, buffer, data]() {
		glNamedBufferSubData(buffer->BufferID, uniform->GetOffset(), uniform->GetSize(), data);
		});
}

void Nut::Shader::ClearUniformBuffers()
{
	Renderer::Submit([]() {
		for (auto& [name, buffer] : s_UniformBuffers)
		{
			glDeleteBuffers(1, &buffer.BufferID);
		}
		s_UniformBuffers.clear();
		});
}

std::shared_ptr<Nut::Shader> Nut::Shader::Create(const std::string name /*= "DefaultShader"*/)
{
	return std::make_shared<Nut::Shader>(name);
}

uint32_t Nut::Shader::GetUniformsLocation(const std::string name)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return -1;
	}
	return m_UniformsLocations[name];
}

std::shared_ptr<Nut::Shader> Nut::Shader::Create(const std::string& name, const std::string& shaderSourcePath)
{
	return std::make_shared<Nut::Shader>(name, shaderSourcePath);
}

void Nut::Shader::UploadUniformInt(const std::string& name, int value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniform1i(location, value);
		});
}

void Nut::Shader::UploadUniformFloat(const std::string& name, float value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniform1f(location, value);
		});
}

void Nut::Shader::UploadUniformInt2(const std::string& name, const glm::ivec2& value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniform2i(location, value.x, value.y);
		});
}

void Nut::Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniform2f(location, value.x, value.y);
		});
}

void Nut::Shader::UploadUniformInt3(const std::string& name, const glm::ivec3& value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniform3i(location, value.x, value.y, value.z);
		});
}

void Nut::Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniform3f(location, value.x, value.y, value.z);
		});
}

void Nut::Shader::UploadUniformInt4(const std::string& name, const glm::ivec4& value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniform4i(location, value.x, value.y, value.z, value.w);
		});
}

void Nut::Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniform4f(location, value.x, value.y, value.z, value.w);
		});
}

void Nut::Shader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
		});
}

void Nut::Shader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	GLuint location = m_UniformsLocations[name];
	Renderer::Submit([location, value]() {
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
		});
}
