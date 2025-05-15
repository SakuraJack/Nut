#include "ntpch.h"
#include "Material.h"

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
	m_ShaderUniformLocations.clear();
	m_ShaderResourceDeclarations.clear();

	m_ShaderUniformLocations = m_Shader->GetUniforms();
	m_ShaderResourceDeclarations = m_Shader->GetResourceDeclarations();
}

void Nut::Material::Bind()
{
	m_Shader->Bind();
}

void Nut::Material::Unbind()
{
	m_Shader->Unbind();
}

void Nut::Material::Set(const std::string& name, int value)
{
	m_Shader->SetUniform(name, value);
}

std::shared_ptr<Nut::Material> Nut::Material::Create(const std::shared_ptr<Shader>& shader, const std::string& name /*= ""*/)
{
	return std::make_shared<Material>(shader, name);
}

std::shared_ptr<Nut::Material> Nut::Material::Copy(const std::shared_ptr<Material>& material, const std::string& name /*= ""*/)
{
	if (material == nullptr) return nullptr;

	auto newMaterial = std::make_shared<Material>(material->m_Shader, name);
	newMaterial->m_ShaderUniformLocations = material->m_ShaderUniformLocations;
	newMaterial->m_ShaderResourceDeclarations = material->m_ShaderResourceDeclarations;

	return newMaterial;
}

std::shared_ptr<Nut::Material> Nut::Material::Create(const std::string& name /*= "Default"*/)
{
	return std::make_shared<Material>(Shader::Create(), name);
}

void Nut::Material::Set(const std::string& name, float value)
{
	m_Shader->SetUniform(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::vec2& value)
{
	m_Shader->SetUniform(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::vec3& value)
{
	m_Shader->SetUniform(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::vec4& value)
{
	m_Shader->SetUniform(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::ivec2& value)
{
	m_Shader->SetUniform(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::ivec3& value)
{
	m_Shader->SetUniform(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::ivec4& value)
{
	m_Shader->SetUniform(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::mat3& value)
{
	m_Shader->SetUniform(name, value);
}

void Nut::Material::Set(const std::string& name, const glm::mat4& value)
{
	m_Shader->SetUniform(name, value);
}

void Nut::Material::Set(const std::string& name, const std::shared_ptr<Texture2D>& texture)
{
	if (m_ShaderResourceDeclarations.find(name) == m_ShaderResourceDeclarations.end()) return;
	if (texture->GetTextureID() && texture->GetTextureSlot() != -1) {
		Set(name, (int)texture->GetTextureSlot());
		// TODO: 优化纹理单元的分配
	}
	else if (texture->GetTextureSlot() == -1) {
		// TODO: 优化纹理单元的分配
		TextureManager::ActiveTextureUnit(texture);
		Set(name, (int)texture->GetTextureSlot());
	}
}

void Nut::Material::Set(const std::string& name, const std::shared_ptr<TextureCube>& texture)
{
	if (m_ShaderResourceDeclarations.find(name) == m_ShaderResourceDeclarations.end()) return;
	if (texture->GetTextureID() && texture->GetTextureSlot() != -1) {
		Set(name, (int)texture->GetTextureSlot());
	}
	else if (texture->GetTextureSlot() == -1) {
		// TODO: 优化纹理单元的分配
		TextureManager::ActiveTextureUnit(texture);
		Set(name, (int)texture->GetTextureSlot());
	}
}

void Nut::Material::Set(const std::string& name, const std::shared_ptr<Image2D>& image)
{

}
