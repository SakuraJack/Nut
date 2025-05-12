#pragma once

#include "glm/glm.hpp"

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

		virtual ImageSpecification& GetSpecification() const = 0;
		virtual const ImageFormat& GetFormat() const = 0;
	};

	class Image2D : public Image
	{

	};
}