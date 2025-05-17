#include "ntpch.h"
#include "TextureManager.h"

void Nut::TextureManager::LoadTextures(const std::string& path)
{
	if (std::filesystem::exists(path)) {
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			TextureSpecification spec;
			if (entry.is_regular_file()) {
				std::shared_ptr<Texture2D> tex = Texture2D::Create(spec, entry.path());
				auto name = entry.path().filename().string();
				m_Textures.insert({ name, tex });
			}
			else {
				std::shared_ptr<TextureCube> tex = TextureCube::Create(spec, entry.path());
				auto name = entry.path().filename().string();
				m_Textures.insert({ name, tex });
			}
		}
	}
	else {
		NUT_ERROR("纹理路径不存在: {0}", path);
		return;
	}
}

std::shared_ptr<Nut::Texture> Nut::TextureManager::GetTexture(const std::string& name)
{
	auto it = m_Textures.find(name);
	if (it != m_Textures.end())
	{
		return it->second;
	}
	return nullptr;
}
