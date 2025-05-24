#include "ntpch.h"
#include "ResourceManager.h"

#include "Core/Log.h"

void Nut::ResourceManager::SetInput(std::string_view name, std::shared_ptr<Texture2D> texture)
{
	if (m_TextureMap.find(name.data()) == m_TextureMap.end())
	{
		NUT_ERROR_TAG("ResourceManager", "未找到着色器中的纹理名称: {0}", name);
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
		NUT_ERROR_TAG("ResourceManager", "未找到着色器中的纹理名称: {0}", name);
		return;
	}
	else
	{
		m_TextureMap[name.data()].second = texture;
	}
}
