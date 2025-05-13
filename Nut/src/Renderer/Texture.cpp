#include "ntpch.h"
#include "Texture.h"
#include "Core/Log.h"
#include "Asset/TextureImporter.h"

#include "glad/glad.h"
#include "Utils/ImageUtils.h"

Nut::Texture2D::Texture2D(const TextureSpecification& spec)
	: m_Specification(spec)
{
	m_ImageData.Allocate((uint32_t)ImageUtils::ImageFormatDataSize(m_Specification.Format) * m_Specification.Width * m_Specification.Height);
	m_ImageData.Zero();

	Invalidate();
}

Nut::Texture2D::Texture2D(const TextureSpecification& spec, Buffer imageData)
	: m_Specification(spec)
{
	if (imageData) {
		auto size = (uint32_t)ImageUtils::ImageFormatDataSize(m_Specification.Format) * m_Specification.Width * m_Specification.Height;
		m_ImageData = Buffer::Copy(imageData.Data, size);
	}
	else {
		auto size = (uint32_t)ImageUtils::ImageFormatDataSize(m_Specification.Format) * m_Specification.Width * m_Specification.Height;
		m_ImageData.Allocate(size);
		m_ImageData.Zero();
	}

	Invalidate();
}

Nut::Texture2D::Texture2D(const TextureSpecification& spec, const std::filesystem::path& filepath)
	: m_Specification(spec), m_FilePath(filepath)
{
	m_ImageData = TextureImporter::ToBufferFromFile(filepath, m_Specification.Format, m_Specification.Width, m_Specification.Height);
	if (!m_ImageData) {
		m_ImageData = TextureImporter::ToBufferFromFile("Resources/Textures/ErrorTexture.png", m_Specification.Format, m_Specification.Width, m_Specification.Height);
	}

	Invalidate();
}

Nut::Texture2D::~Texture2D()
{
	m_ImageData.Release();
}

void Nut::Texture2D::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_TextureID);
}

void Nut::Texture2D::Invalidate()
{
	if (m_TextureID) {
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = 0;
	}

	if (m_Specification.GenerateMipMaps)
		m_Specification.MipLevels = (uint32_t)std::floor(std::log2(std::max(m_Specification.Width, m_Specification.Height))) + 1;
	else
		m_Specification.MipLevels = 1;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
	glTextureStorage2D(m_TextureID, m_Specification.MipLevels,
		ImageUtils::ImageFormatToGLInternalFormat(m_Specification.Format), m_Specification.Width, m_Specification.Height);

	glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, ImageUtils::ImageFilterToGL(m_Specification.SamplerFilter));
	glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, ImageUtils::ImageFilterToGL(m_Specification.SamplerFilter));
	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, ImageUtils::ImageWrapToGL(m_Specification.SamplerWrap));
	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, ImageUtils::ImageWrapToGL(m_Specification.SamplerWrap));

	glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Specification.Width, m_Specification.Height,
		ImageUtils::ImageFormatToOpenGLFormat(m_Specification.Format),
		ImageUtils::ImageFormatToGLByteType(m_Specification.Format),
		m_ImageData.Data);
}

void Nut::Texture2D::Resize(const glm::uvec2& size)
{
	Resize(size.x, size.y);
}

void Nut::Texture2D::SetData(const void* data, uint32_t size)
{
	m_ImageData.Release();
	m_ImageData.Allocate(size);
	m_ImageData.Write(data, size, 0);
}

void Nut::Texture2D::Resize(const uint32_t width, const uint32_t height)
{
	if (width == m_Specification.Width && height == m_Specification.Height)
		return;

	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalidate();
}

std::shared_ptr<Nut::Texture2D> Nut::Texture2D::Create(const TextureSpecification& spec)
{
	return std::make_shared<Texture2D>(spec);
}

std::shared_ptr<Nut::Texture2D> Nut::Texture2D::Create(const TextureSpecification& spec, const std::filesystem::path& filepath)
{
	return std::make_shared<Texture2D>(spec, filepath);
}

std::shared_ptr<Nut::Texture2D> Nut::Texture2D::Create(const TextureSpecification& spec, Buffer imageData)
{
	return std::make_shared<Texture2D>(spec, imageData);
}

void Nut::Texture::Bind(uint32_t slot, const std::shared_ptr<Texture>& texture)
{
	if (texture) {
		texture->Bind(slot);
	}
	else {
		glBindTextureUnit(slot, 0);
	}
}

void Nut::Texture::Unbind(uint32_t slot)
{
	glBindTextureUnit(slot, 0);
}
