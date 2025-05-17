#pragma once

#include "Renderer/Texture.h"

namespace Nut {

	// TODO: ��ʱ��������������� ֮���Ƴ�
	class TextureManager
	{
	public:
		static void LoadTextures(const std::string& path); // ��������
		static std::shared_ptr<Texture> GetTexture(const std::string& name);

	private:
		inline static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
	};
}