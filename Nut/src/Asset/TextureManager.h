#pragma once

#include "Renderer/Texture.h"

namespace Nut {

	class TextureManager
	{
		// TODO: ��д����Ԫ���� ��ʱʹ�������δ���䵥Ԫ
	public:
		static void Init(uint32_t count);

		static bool TextureActivated(const std::shared_ptr<Texture>& texture);
		static void ActiveTextureUnit(const std::shared_ptr<Texture>& texture);
		static uint32_t GetLeastUnusedTextureUnit();
	private:
		inline static std::vector<std::shared_ptr<Texture>> s_ActiveTextureUnits;
	};
}
