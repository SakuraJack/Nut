#pragma once
#include "Image.h"

namespace Nut {

	struct TextureSpecification
	{
		ImageFormat Format = ImageFormat::RGBA;
		uint32_t Width = 1;
		uint32_t Height = 1;
		TextureWrap SamplerWrap = TextureWrap::Repeat;
		TextureFilter SamplerFilter = TextureFilter::Linear;
		TextureType Type = TextureType::Texture2D;

		bool GenerateMipMaps = false;
		uint32_t MipLevels = 1;

		std::string DebugName;
	};

	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual ImageFormat GetFormat() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::uvec2 GetSize() const = 0;
		virtual uint32_t GetMipLevel() const = 0;
		virtual TextureType GetType() const = 0;

		static void Bind(uint32_t slot, const std::shared_ptr<Texture>& texture);
		static void Unbind(uint32_t slot);
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(const TextureSpecification& spec);
		Texture2D(const TextureSpecification& spec, Buffer imageData);
		Texture2D(const TextureSpecification& spec, const std::filesystem::path& filepath);
		~Texture2D() override;

		virtual void Bind(uint32_t slot) const override;

		void Invalidate();

		void Resize(const glm::uvec2& size);
		void Resize(const uint32_t width, const uint32_t height);
		void SetData(const void* data, uint32_t size);

		virtual ImageFormat GetFormat() const override { return m_Specification.Format; }
		virtual uint32_t GetWidth() const override { return m_Specification.Width; }
		virtual uint32_t GetHeight() const override { return m_Specification.Height; }
		virtual glm::uvec2 GetSize() const override { return { m_Specification.Width, m_Specification.Height }; }
		virtual uint32_t GetMipLevel() const override { return m_Specification.MipLevels; }
		virtual TextureType GetType() const override { return m_Specification.Type; }

		static std::shared_ptr<Texture2D> Create(const TextureSpecification& spec);
		static std::shared_ptr<Texture2D> Create(const TextureSpecification& spec, Buffer imageData);
		static std::shared_ptr<Texture2D> Create(const TextureSpecification& spec, const std::filesystem::path& filepath);

	private:
		TextureSpecification m_Specification;
		Buffer m_ImageData;
		std::filesystem::path m_FilePath;
		uint32_t m_TextureID;
		uint32_t m_TextureSlot = 0;
		uint32_t m_SamplerID = 0;
	};
}