#include "ntpch.h"
#include "Material.h"
#include "Renderer.h"
#include "Asset/TextureManager.h"

Nut::Material::Material()
{
	m_Shader = Shader::Create();
	m_Name = "Default";
	Invalidate();
}

Nut::Material::Material(const std::shared_ptr<Shader>& shader, const std::string& name /*= "Default"*/)
	: m_Shader(shader), m_Name(name)
{
	Invalidate();
}

void Nut::Material::Invalidate()
{
}

void Nut::Material::Bind()
{
	m_Shader->Bind();
}

void Nut::Material::Unbind()
{
	m_Shader->Unbind();
}

std::shared_ptr<Nut::Material> Nut::Material::Create(const std::string& name /*= "Default"*/)
{
	return std::make_shared<Material>(Shader::Create(), name);
}

std::shared_ptr<Nut::Material> Nut::Material::Create(const std::shared_ptr<Shader>& shader, const std::string& name /*= ""*/)
{
	return std::make_shared<Material>(shader, name);
}

std::shared_ptr<Nut::Material> Nut::Material::Copy(const std::shared_ptr<Material>& material, const std::string& name /*= ""*/)
{
	if (material == nullptr) return nullptr;
	auto newMaterial = std::make_shared<Material>(material->m_Shader, name);
	return newMaterial;
}

void Nut::Material::Set(const std::string& name, int value)
{
	Set<int>(name, value);
}

void Nut::Material::Set(const std::string& name, float value)
{
	Set<float>(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::vec2& value)
{
	Set<glm::vec2>(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::vec3& value)
{
	Set<glm::vec3>(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::vec4& value)
{
	Set<glm::vec4>(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::ivec2& value)
{
	Set<glm::ivec2>(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::ivec3& value)
{
	Set<glm::ivec3>(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::ivec4& value)
{
	Set<glm::ivec4>(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::mat3& value)
{
	Set<glm::mat3>(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::mat4& value)
{
	Set<glm::mat4>(name, value);
}

const Nut::ShaderUniform* Nut::Material::FindUniformInUniformBuffer(const std::string& name)
{
	const auto& shaderBuffers = m_Shader->GetUniformBuffers();

	if (shaderBuffers.size() > 0)
	{
		for (const auto& [bufferName, buffer] : shaderBuffers)
		{
			auto it = buffer.Uniforms.find(name);
			if (it != buffer.Uniforms.end())
			{
				return &it->second;
			}
		}
	}
	return nullptr;
}

const Nut::ShaderUniform* Nut::Material::FindUniformInStorageBuffer(const std::string& name)
{
	const auto& shaderBuffers = m_Shader->GetStorageBuffers();

	if (shaderBuffers.size() > 0)
	{
		for (const auto& [bufferName, buffer] : shaderBuffers)
		{
			auto it = buffer.Uniforms.find(name);
			if (it != buffer.Uniforms.end())
			{
				return &it->second;
			}
		}
	}
	return nullptr;
}

const Nut::ShaderResourceDeclaration* Nut::Material::FindResourceInUniformBuffer(const std::string& name)
{
	const auto& shaderResources = m_Shader->GetResourceDeclarations();
	if (shaderResources.find(name) != shaderResources.end())
	{
		return &shaderResources.at(name);
	}
	return nullptr;
}

void Nut::Material::Set(const std::string& name, const std::shared_ptr<Texture2D>& texture)
{

}

void Nut::Material::Set(const std::string& name, const std::shared_ptr<TextureCube>& texture)
{

}

void Nut::Material::Set(const std::string& name, const std::shared_ptr<Image2D>& image)
{

}

float& Nut::Material::GetFloat(const std::string& name)
{
	return Get<float>(name);
}

int& Nut::Material::GetInt(const std::string& name)
{
	return Get<int>(name);
}

glm::vec2& Nut::Material::GetVec2(const std::string& name)
{
	return Get<glm::vec2>(name);
}

glm::vec3& Nut::Material::GetVec3(const std::string& name)
{
	return Get<glm::vec3>(name);
}

glm::vec4& Nut::Material::GetVec4(const std::string& name)
{
	return Get<glm::vec4>(name);
}

glm::ivec2& Nut::Material::GetIVec2(const std::string& name)
{
	return Get<glm::ivec2>(name);
}

glm::ivec3& Nut::Material::GetIVec3(const std::string& name)
{
	return Get<glm::ivec3>(name);
}

glm::ivec4& Nut::Material::GetIVec4(const std::string& name)
{
	return Get<glm::ivec4>(name);
}

glm::mat3& Nut::Material::GetMat3(const std::string& name)
{
	return Get<glm::mat3>(name);
}

glm::mat4& Nut::Material::GetMat4(const std::string& name)
{
	return Get<glm::mat4>(name);
}

uint32_t& Nut::Material::GetUInt(const std::string& name)
{
	return Get<uint32_t>(name);
}

bool& Nut::Material::GetBool(const std::string& name)
{
	return Get<bool>(name);
}


