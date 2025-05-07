#include "ntpch.h"
#include "Shader.h"
#include "Core/Log.h"
#include "Tool/ShaderCompilerTools.h"

namespace Nut {
	std::unordered_map<std::string, ShaderUniformBuffer> Shader::s_UniformBuffers = {};
	std::unordered_map<std::string, ShaderStorageBuffer> Shader::s_StorageBuffers = {};
}

Nut::Shader::Shader(const std::string name /*= "DefaultShader"*/)
	: m_Name(name), m_AssetPath("Resources/Shaders/" + name + ".glsl")
{
	ShaderCompiler compiler(m_AssetPath);
	if (!compiler.Compile(m_ShaderID)) {
		NUT_ERROR_TAG("Shader", "Failed to compile shader: {0}", name);
	}
	else {
		m_ShaderSource = std::move(compiler.m_ShaderSource);
		m_SPIRVData = std::move(compiler.m_ShaderBinaries);
		m_UniformsLocations = std::move(compiler.m_Uniforms);
	}
}

Nut::Shader::Shader(const std::string& name, const std::string& shaderSourcePath)
	: m_Name(name), m_AssetPath(shaderSourcePath)
{
	ShaderCompiler compiler(m_AssetPath);
	if (!compiler.Compile(m_ShaderID)) {
		NUT_ERROR_TAG("Shader", "Failed to compile shader: {0}", name);
	}
	else {
		m_ShaderSource = std::move(compiler.m_ShaderSource);
		m_SPIRVData = std::move(compiler.m_ShaderBinaries);
		m_UniformsLocations = std::move(compiler.m_Uniforms);
	}
}

void Nut::Shader::Reload(const std::string& shaderSourcePath)
{
	ShaderCompiler::Compile(shared_from_this());
}

void Nut::Shader::ForceReload(const std::string& shaderSourcePath)
{
	ShaderCompiler::Compile(shared_from_this(), true);
}

void Nut::Shader::Bind()
{
	glUseProgram(m_ShaderID);
}

void Nut::Shader::Unbind()
{
	glUseProgram(0);
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

void Nut::Shader::SetUniformBuffer(const std::string& uniformBufferName, const void* data, unsigned int size, unsigned int offset /*= 0*/)
{
	glNamedBufferSubData(s_UniformBuffers[uniformBufferName].BufferID, offset, size, data);
}

void Nut::Shader::ClearUniformBuffers()
{
	for (auto& [name, buffer] : s_UniformBuffers)
	{
		glDeleteBuffers(1, &buffer.BufferID);
	}
	s_UniformBuffers.clear();
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
	else {
		return m_UniformsLocations[name];
	}
}

std::shared_ptr<Nut::Shader> Nut::Shader::Create(const std::string& name, const std::string& shaderSourcePath)
{
	return std::make_shared<Nut::Shader>(name, shaderSourcePath);
}

void Nut::Shader::UploadUniformInt(const std::string& name, int value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniform1i(location, value);
}

void Nut::Shader::UploadUniformFloat(const std::string& name, float value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniform1f(location, value);
}

void Nut::Shader::UploadUniformInt2(const std::string& name, const glm::ivec2& value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniform2i(location, value.x, value.y);
}

void Nut::Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniform2f(location, value.x, value.y);
}

void Nut::Shader::UploadUniformInt3(const std::string& name, const glm::ivec3& value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniform3i(location, value.x, value.y, value.z);
}

void Nut::Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniform3f(location, value.x, value.y, value.z);
}

void Nut::Shader::UploadUniformInt4(const std::string& name, const glm::ivec4& value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniform4i(location, value.x, value.y, value.z, value.w);
}

void Nut::Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Nut::Shader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void Nut::Shader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
{
	GLuint location = glGetUniformLocation(m_ShaderID, name.c_str());
	if (location == -1)
	{
		NUT_ERROR_TAG("Shader", "Uniform not found: {0}", name);
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}
