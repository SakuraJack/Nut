#include "ntpch.h"
#include "ResourceManager.h"

Nut::ResourceManager::ResourceManager(const ResourceManager& other)
{
	m_TextureMap = other.m_TextureMap;
}

Nut::ResourceManager Nut::ResourceManager::Copy(const ResourceManager& other)
{
	ResourceManager resourceManager(other);
	return resourceManager;
}

void Nut::ResourceManager::SetInput(std::string_view name, std::shared_ptr<Texture2D> texture)
{
	if (m_TextureMap.find(name.data()) == m_TextureMap.end())
	{
		NUT_CORE_ERROR_TAG("ResourceManager", "未找到着色器中的纹理名称: {0}", name);
		return;
	}
	else
	{
		m_TextureMap[name.data()].second = texture;
	}
}

void Nut::ResourceManager::SetInput(std::string_view name, std::shared_ptr<TextureCube> texture)
{
	if (m_TextureMap.find(name.data()) == m_TextureMap.end())
	{
		NUT_CORE_ERROR_TAG("ResourceManager", "未找到着色器中的纹理名称: {0}", name);
		return;
	}
	else
	{
		m_TextureMap[name.data()].second = texture;
	}
}

void Nut::ResourceManager::SetInput(std::string_view name, std::shared_ptr<Texture2D> texture, uint32_t location) 
{
	m_TextureMap[name.data()] = { location, texture };
}

void Nut::ResourceManager::SetInput(std::string_view name, std::shared_ptr<TextureCube> texture, uint32_t location)
{
	m_TextureMap[name.data()] = { location, texture };
}
