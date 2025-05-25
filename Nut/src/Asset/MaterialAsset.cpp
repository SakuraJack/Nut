#include "ntpch.h"
#include "MaterialAsset.h"
#include "Renderer/Renderer.h"

namespace Nut {

	static const std::string s_AlbedoColorUniform = "AlbedoColor";
	static const std::string s_UseNormalMapUniform = "UseNormalMap";
	static const std::string s_MetalnessUniform = "Metalness";
	static const std::string s_RoughnessUniform = "Roughness";
	static const std::string s_EmissionUniform = "Emission";
	static const std::string s_TransparencyUniform = "Transparency";

	static const std::string s_AlbedoMapUniform = "u_AlbedoTexture";
	static const std::string s_NormalMapUniform = "u_NormalTexture";
	static const std::string s_MetalnessMapUniform = "u_MetalnessTexture";
	static const std::string s_RoughnessMapUniform = "u_RoughnessTexture";
}

Nut::MaterialAsset::MaterialAsset(bool transparent)
	: m_Transparent(transparent)
{
	Handle = {};

	if (transparent)
		m_Material = Material::Create("NutPBRTransparent");
	else 
		m_Material = Material::Create("NutPBR");
}

Nut::MaterialAsset::MaterialAsset(std::shared_ptr<Material> material)
{
	Handle = {};
	m_Material = Material::Copy(material);
}

Nut::MaterialAsset::~MaterialAsset()
{
}

glm::vec3& Nut::MaterialAsset::GetAlbedoColor()
{
	return m_Material->GetVec3(s_AlbedoColorUniform);
}

void Nut::MaterialAsset::SetAlbedoColor(const glm::vec3& color)
{
	m_Material->Set(s_AlbedoColorUniform, color);
}

float& Nut::MaterialAsset::GetMetalness()
{
	return m_Material->GetFloat(s_MetalnessUniform);
}

void Nut::MaterialAsset::SetMetalness(float value)
{
	m_Material->Set(s_MetalnessUniform, value);
}

float& Nut::MaterialAsset::GetRoughness()
{
	return m_Material->GetFloat(s_RoughnessUniform);
}

void Nut::MaterialAsset::SetRoughness(float value)
{
	m_Material->Set(s_RoughnessUniform, value);
}

float& Nut::MaterialAsset::GetEmission()
{
	return m_Material->GetFloat(s_EmissionUniform);
}

void Nut::MaterialAsset::SetEmission(float value)
{
	m_Material->Set(s_EmissionUniform, value);
}

std::shared_ptr<Nut::Texture2D> Nut::MaterialAsset::GetAlbedoMap()
{
	return m_Material->GetResource<Texture2D>(s_AlbedoMapUniform);
}

void Nut::MaterialAsset::SetAlbedoMap(std::shared_ptr<Texture2D> texture)
{
	m_Material->Set(s_AlbedoMapUniform, texture);
}

void Nut::MaterialAsset::ClearAlbedoMap()
{
	m_Material->Set(s_AlbedoMapUniform, Renderer::GetWhiteTexture());
}

std::shared_ptr<Nut::Texture2D> Nut::MaterialAsset::GetNormalMap()
{
	return m_Material->GetResource<Texture2D>(s_NormalMapUniform);
}

void Nut::MaterialAsset::SetNormalMap(std::shared_ptr<Texture2D> texture)
{
	m_Material->Set(s_NormalMapUniform, texture);
}

bool Nut::MaterialAsset::UseNormalMap()
{
	return m_Material->GetBool(s_UseNormalMapUniform);
}

void Nut::MaterialAsset::SetUseNormalMap(bool use)
{
	m_Material->Set(s_UseNormalMapUniform, use);
}

void Nut::MaterialAsset::ClearNormalMap()
{
	m_Material->Set(s_NormalMapUniform, Renderer::GetWhiteTexture());
}

std::shared_ptr<Nut::Texture2D> Nut::MaterialAsset::GetMetalnessMap()
{
	return m_Material->GetResource<Texture2D>(s_MetalnessMapUniform);
}

void Nut::MaterialAsset::SetMetalnessMap(std::shared_ptr<Texture2D> texture)
{
	m_Material->Set(s_MetalnessMapUniform, texture);
}

void Nut::MaterialAsset::ClearMetalnessMap()
{
	m_Material->Set(s_MetalnessMapUniform, Renderer::GetWhiteTexture());
}

std::shared_ptr<Nut::Texture2D> Nut::MaterialAsset::GetRoughnessMap()
{
	return m_Material->GetResource<Texture2D>(s_RoughnessMapUniform);
}

void Nut::MaterialAsset::SetRoughnessMap(std::shared_ptr<Texture2D> texture)
{
	m_Material->Set(s_RoughnessMapUniform, texture);
}

void Nut::MaterialAsset::ClearRoughnessMap()
{
	m_Material->Set(s_RoughnessMapUniform, Renderer::GetWhiteTexture());
}

float& Nut::MaterialAsset::GetTransparency()
{
	return m_Material->GetFloat(s_TransparencyUniform);
}

void Nut::MaterialAsset::SetTransparency(float value)
{
	m_Material->Set(s_TransparencyUniform, value);
}

Nut::MaterialTable::MaterialTable(uint32_t materialCount)
	: m_MaterialCount(materialCount)
{
}

Nut::MaterialTable::MaterialTable(std::shared_ptr<Nut::MaterialTable> other)
	: m_MaterialCount(other->m_MaterialCount)
{
	const auto& meshMaterials = other->GetMaterials();
	for (auto [index, materialAsset] : meshMaterials) {
		SetMaterial(index, materialAsset);
	}
}

void Nut::MaterialTable::SetMaterial(uint32_t index, AssetHandle material)
{
	m_Materials[index] = material;
	if (index >= m_MaterialCount)
		m_MaterialCount = index + 1;
}

void Nut::MaterialTable::ClearMaterial(uint32_t index)
{
	if (m_Materials.find(index) == m_Materials.end())
	{
		NUT_ERROR_TAG("MaterialTable", "未找到材质索引: {0}", index);
		return;
	}
	m_Materials.erase(index);
	if (index >= m_MaterialCount)
		m_MaterialCount = index + 1;
}

Nut::AssetHandle Nut::MaterialTable::GetMaterial(uint32_t index) const
{
	if (m_Materials.find(index) == m_Materials.end())
	{
		NUT_ERROR_TAG("MaterialTable", "未找到材质索引: {0}", index);
		return {};
	}
	return m_Materials.at(index);
}

void Nut::MaterialTable::Clear()
{
	m_Materials.clear();
	m_MaterialCount = 0;
}
