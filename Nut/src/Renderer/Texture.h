#pragma once
#include "Image.h"

namespace Nut {

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA;
		TextureWrap SamplerWrap = TextureWrap::Repeat;
		TextureFilter SamplerFilter = TextureFilter::Linear;
		TextureType Type = TextureType::Texture2D;

		bool GenerateMipMaps = false;
		uint32_t MipLevels = 1;

		std::string DebugName;
	};

	// TODO: 考虑之后添加无绑定纹理以及纹理数组
	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void Unbind() = 0;

		virtual ImageFormat GetFormat() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::uvec2 GetSize() const = 0;
		virtual uint32_t GetMipLevel() const = 0;
		virtual TextureType GetType() const = 0;

	public:
		// TODO: 移入AssetManager 统一管理所有资源类
		inline static std::vector<std::shared_ptr<Texture>> s_Textures;
	};

	class Texture2D : public Texture, public std::enable_shared_from_this<Texture2D>
	{
	public:
		Texture2D(const TextureSpecification& spec);
		Texture2D(const TextureSpecification& spec, Buffer imageData);
		Texture2D(const TextureSpecification& spec, const std::filesystem::path& filepath);
		~Texture2D() override;

		void Bind(uint32_t slot) override;
		void Unbind() override;

		void Invalidate();

		void Resize(const glm::uvec2& size);
		void Resize(const uint32_t width, const uint32_t height);
		void SetData(const void* data, uint32_t size);

		ImageFormat GetFormat() const override { return m_Specification.Format; }
		uint32_t GetWidth() const override { return m_Specification.Width; }
		uint32_t GetHeight() const override { return m_Specification.Height; }
		glm::uvec2 GetSize() const override { return { m_Specification.Width, m_Specification.Height }; }
		uint32_t GetMipLevel() const override { return m_Specification.MipLevels; }
		TextureType GetType() const override { return m_Specification.Type; }
		RenderID GetTextureID() const { return m_TextureID; }
		uint32_t GetTextureSlot() const { return m_TextureSlot; }

		static std::shared_ptr<Texture2D> Create(const TextureSpecification& spec);
		static std::shared_ptr<Texture2D> Create(const TextureSpecification& spec, Buffer imageData);
		static std::shared_ptr<Texture2D> Create(const TextureSpecification& spec, const std::filesystem::path& filepath);

	private:
		TextureSpecification m_Specification;
		Buffer m_ImageData;
		std::filesystem::path m_FilePath;
		RenderID m_TextureID;
		uint32_t m_TextureSlot = -1;
	};

	class TextureCube : public Texture, public std::enable_shared_from_this<TextureCube>
	{
	public:
	public:
		TextureCube(const TextureSpecification& spec);
		TextureCube(const TextureSpecification& spec, std::vector<Buffer> imageData);
		TextureCube(const TextureSpecification& spec, const std::filesystem::path& filepath);
		~TextureCube() override;

		void Bind(uint32_t slot) override;
		void Unbind() override;

		void Invalidate();

		void SetData(const void* data, uint32_t size, uint32_t faceindex);

		ImageFormat GetFormat() const override { return m_Specification.Format; }
		uint32_t GetWidth() const override { return m_Specification.Width; }
		uint32_t GetHeight() const override { return m_Specification.Height; }
		glm::uvec2 GetSize() const override { return { m_Specification.Width, m_Specification.Height }; }
		uint32_t GetMipLevel() const override { return m_Specification.MipLevels; }
		TextureType GetType() const override { return m_Specification.Type; }
		RenderID GetTextureID() const { return m_TextureID; }

		static std::shared_ptr<TextureCube> Create(const TextureSpecification& spec);
		static std::shared_ptr<TextureCube> Create(const TextureSpecification& spec, std::vector<Buffer> imageDatas);
		static std::shared_ptr<TextureCube> Create(const TextureSpecification& spec, const std::filesystem::path& filepath);

	private:
		TextureSpecification m_Specification;
		std::vector<Buffer> m_ImageDatas;  // 面数据 顺序: +X, -X, +Y, -Y, +Z, -Z
		std::filesystem::path m_FilePath;
		RenderID m_TextureID;
	};
}