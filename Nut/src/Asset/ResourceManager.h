#pragma once

#include "Renderer/Texture.h"

namespace Nut {

	class ResourceManager
	{
	public:
		ResourceManager() = default;

		void SetInput(std::string_view name, std::shared_ptr<Texture2D> texture);
		void SetInput(std::string_view name, std::shared_ptr<TextureCube> texture);

		template<typename T>
		std::shared_ptr<T> GetInput(std::string_view name)
		{
			auto it = m_TextureMap.find(name.data());
			if (it != m_TextureMap.end())
				return std::static_pointer_cast<T>(it->second.second);
			else
				return nullptr;
		}
	private:
		std::map<std::string, std::pair<uint32_t, std::shared_ptr<Texture>>> m_TextureMap;
	};
}