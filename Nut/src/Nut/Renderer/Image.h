#pragma once

#include "glm/glm.hpp"
#include "Nut/Core/Buffer.h"

namespace Nut {

	enum class ImageFormat
	{
		None = 0,
		RED8,
		RED16,
		RED32,
		RED32F,
		RG8,
		RG16F,
		RG32F,
		RGB,
		RGBA,
		RGBA16F,
		RGBA32F,

		SRGB,

		DEPTH32FSTENCIL8UINT,
		DEPTH32F,
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8,
	};

	enum class ImageUsage
	{
		None = 0,
		Texture,
		Attachment,
		Storage,
	};

	enum class TextureWrap
	{
		None = 0,
		Clamp,
		Repeat,
	};

	enum class TextureFilter
	{
		None = 0,
		Linear,
		Nearest,
		Cubic,
	};

	enum class TextureType
	{
		None = 0,
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube,
	};

	struct ImageSpecification
	{
		std::string Name;

		ImageFormat Format = ImageFormat::RGBA;
		ImageUsage Usage = ImageUsage::Texture;
		uint32_t Width = 1;
		uint32_t Height = 1;
		uint32_t Mips = 1;
	};

	namespace Utils
	{
		inline bool IsDepthFormat(ImageFormat format)
		{
			if (format == ImageFormat::DEPTH32FSTENCIL8UINT ||
				format == ImageFormat::DEPTH32F ||
				format == ImageFormat::DEPTH24STENCIL8)
			{
				return true;
			}
		}
	}

	class Image
	{
	public:
		virtual ~Image() = default;

		virtual void Resize(const uint32_t width, const uint32_t height) = 0;
		virtual void Invalidate() = 0;
		virtual void Release() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual glm::uvec2 GetSize() const = 0;

		virtual float GetAspectRatio() const = 0;

		virtual const ImageSpecification& GetSpecification() = 0;
		virtual const ImageFormat& GetFormat() = 0;
	};

	class Image2D : public Image
	{
	public:
		Image2D(const ImageSpecification& spec, Buffer buffer);
		Image2D(const ImageSpecification& spec, const void* data = nullptr);
		virtual ~Image2D() override;

		static std::shared_ptr<Image2D> Create(const ImageSpecification& spec, Buffer buffer);
		static std::shared_ptr<Image2D> Create(const ImageSpecification& spec, const void* data = nullptr);
		void Resize(const glm::uvec2& size);
		void Resize(const uint32_t width, const uint32_t height) override;

		void Invalidate() override;
		void Release() override;

		inline uint32_t GetWidth() const override { return m_Specification.Width; }
		inline uint32_t GetHeight() const override { return m_Specification.Height; }
		inline glm::uvec2 GetSize() const override { return { m_Specification.Width, m_Specification.Height }; }

		float GetAspectRatio() const override { return m_Specification.Width / m_Specification.Height; }

		const ImageSpecification& GetSpecification() override { return m_Specification; }
		const ImageFormat& GetFormat() override { return m_Specification.Format; }

		Buffer GetBuffer() const { return m_ImageData; }
		Buffer& GetBuffer() { return m_ImageData; }
		RenderID& GetImageID() { return m_ImageID; }

	private:
		Buffer m_ImageData;
		ImageSpecification m_Specification;

		RenderID m_ImageID = 0; // OpenGL texture ID or Vulkan image handle, etc.
	};
}