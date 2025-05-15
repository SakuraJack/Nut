#include "ntpch.h"
#include "TextureManager.h"

void Nut::TextureManager::Init(uint32_t count)
{
	s_ActiveTextureUnits.assign(count, nullptr);
}

bool Nut::TextureManager::TextureActivated(const std::shared_ptr<Texture>& texture)
{
	if (texture == nullptr) return false;
	for (const auto& tex : s_ActiveTextureUnits)
	{
		if (tex == texture) return true;
	}
	return false;
}

void Nut::TextureManager::ActiveTextureUnit(const std::shared_ptr<Texture>& texture)
{
	if (TextureActivated(texture)) return;
	uint32_t unit = GetLeastUnusedTextureUnit();
	if (unit >= s_ActiveTextureUnits.size()) {
		NUT_ERROR("TextureManager: No available texture unit!");
		return;
	}
	else {
		s_ActiveTextureUnits[unit] = texture;
		texture->Bind(unit);
	}
}

uint32_t Nut::TextureManager::GetLeastUnusedTextureUnit()
{
	for (uint32_t i = 0; i < s_ActiveTextureUnits.size(); i++)
	{
		if (s_ActiveTextureUnits[i] == nullptr)
		{
			return i;
		}
	}
	return s_ActiveTextureUnits.size();
}
