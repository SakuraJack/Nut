#include "ntpch.h"
#include "MaterialAsset.h"

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
	//return m_Material->
}

