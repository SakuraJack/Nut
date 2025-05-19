#pragma once

#include "Renderer/Texture.h"

namespace Nut {

	// TODO: 暂时先用这个管理纹理 之后移除
	class TextureManager
	{
	public:
		static void LoadTextures(const std::string& path); // 载入纹理
		static std::shared_ptr<Texture> GetTexture(const std::string& name);

	private:
		inline static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
	};
}