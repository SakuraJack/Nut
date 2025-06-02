#include "ntpch.h"
#include "Texture.h"
#include "Core/Log.h"
#include "Asset/TextureImporter.h"

#include "glad/glad.h"
#include "Utils/ImageUtils.h"
#include "Utils/FileUtils.h"
#include "Renderer.h"

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
	if (m_TextureID) {
		RenderID textureID = m_TextureID;
		uint32_t textureSlot = m_TextureSlot;;
		Renderer::Submit([textureID, textureSlot] {
			if (textureSlot != -1)
				glBindTextureUnit(textureSlot, 0);
			glDeleteTextures(1, &textureID);
			});
	}
}

void Nut::Texture2D::Bind(uint32_t slot)
{
	m_TextureSlot = slot;
	Renderer::Submit([this, slot]() {
		glBindTextureUnit(slot, m_TextureID);
		});
}

void Nut::Texture2D::Unbind()
{
	if (m_TextureSlot != -1) {
		Renderer::Submit([this]() {
			glBindTextureUnit(m_TextureSlot, 0);
			m_TextureSlot = -1;
			});
	}
}

void Nut::Texture2D::Invalidate()
{
	Renderer::Submit([this]() {
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
		});
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

	if (m_TextureID) {
		Renderer::Submit([this]() {
			glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Specification.Width, m_Specification.Height,
				ImageUtils::ImageFormatToOpenGLFormat(m_Specification.Format),
				ImageUtils::ImageFormatToGLByteType(m_Specification.Format),
				m_ImageData.Data);
			});
	}
	else {
		Invalidate();
	}
}

void Nut::Texture2D::Resize(const uint32_t width, const uint32_t height)
{
	if (width == m_Specification.Width && height == m_Specification.Height)
		return;

	m_Specification.Width = width;
	m_Specification.Height = height;
	m_ImageData.Release();
	m_ImageData.Allocate((uint32_t)ImageUtils::ImageFormatDataSize(m_Specification.Format) * m_Specification.Width * m_Specification.Height);
	m_ImageData.Zero();

	Invalidate();
}

std::shared_ptr<Nut::Texture2D> Nut::Texture2D::Create(const TextureSpecification& spec)
{
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(spec);
	s_Textures.push_back(texture);
	return texture;
}

std::shared_ptr<Nut::Texture2D> Nut::Texture2D::Create(const TextureSpecification& spec, const std::filesystem::path& filepath)
{
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(spec, filepath);
	s_Textures.push_back(texture);
	return texture;
}

std::shared_ptr<Nut::Texture2D> Nut::Texture2D::Create(const TextureSpecification& spec, Buffer imageData)
{
	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(spec, imageData);
	s_Textures.push_back(texture);
	return texture;
}

Nut::TextureCube::TextureCube(const TextureSpecification& spec)
	: m_Specification(spec)
{
	m_ImageDatas.assign(6, Buffer());
	for (Buffer& buffer : m_ImageDatas) {
		buffer.Allocate((uint32_t)ImageUtils::ImageFormatDataSize(m_Specification.Format) * m_Specification.Width * m_Specification.Height);
		buffer.Zero();
	}

	Invalidate();
}

Nut::TextureCube::~TextureCube()
{
	for (Buffer& buffer : m_ImageDatas) {
		buffer.Release();
	}
	if (m_TextureID) {
		RenderID textureID = m_TextureID;
		Renderer::Submit([textureID]() {
			glDeleteTextures(1, &textureID);
			});
	}
}

Nut::TextureCube::TextureCube(const TextureSpecification& spec, std::vector<Buffer> imageData)
	: m_Specification(spec)
{
	if (imageData.size() != 6) {
		NUT_ERROR_TAG("Texture", "立方体纹理必须有六幅图像, 但只获得了 {0}", m_ImageDatas.size());
		return;
	}

	m_ImageDatas.assign(6, Buffer());
	for (int i = 0; i < 6; ++i) {
		if (imageData[i]) {
			auto size = (uint32_t)ImageUtils::ImageFormatDataSize(m_Specification.Format) * m_Specification.Width * m_Specification.Height;
			m_ImageDatas[i] = Buffer::Copy(imageData[i].Data, size);
		}
		else {
			auto size = (uint32_t)ImageUtils::ImageFormatDataSize(m_Specification.Format) * m_Specification.Width * m_Specification.Height;
			m_ImageDatas[i].Allocate(size);
			m_ImageDatas[i].Zero();
		}
	}

	Invalidate();
}

Nut::TextureCube::TextureCube(const TextureSpecification& spec, const std::filesystem::path& filepath)
	: m_Specification(spec), m_FilePath(filepath)
{
	auto files = FileUtils::ListFilesInDirectory(filepath);
	std::sort(files.begin(), files.end(), [](const std::filesystem::path& a, const std::filesystem::path& b) {
		return a.filename() < b.filename();
		});

	if (files.size() != 6) {
		NUT_ERROR_TAG("Texture", "立方体纹理必须有六幅图像, 但只获得了 {0}", files.size());
		return;
	}

	m_ImageDatas.assign(6, Buffer());
	for (int i = 0; i < 6; ++i) {
		m_ImageDatas[i] = TextureImporter::ToBufferFromFile(files[i], m_Specification.Format, m_Specification.Width, m_Specification.Height);
		if (!m_ImageDatas[i]) {
			m_ImageDatas[i] = TextureImporter::ToBufferFromFile("Resources/Textures/ErrorTexture.png", m_Specification.Format, m_Specification.Width, m_Specification.Height);
		}
	}

	Invalidate();
}

void Nut::TextureCube::Invalidate()
{
	Renderer::Submit([this]() {
		if (m_TextureID) {
			glDeleteTextures(1, &m_TextureID);
			m_TextureID = 0;
		}

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_TextureID);
		if (m_Specification.GenerateMipMaps)
			m_Specification.MipLevels = (uint32_t)std::floor(std::log2(std::max(m_Specification.Width, m_Specification.Height))) + 1;
		else
			m_Specification.MipLevels = 1;

		glTextureStorage2D(m_TextureID, m_Specification.MipLevels,
			ImageUtils::ImageFormatToGLInternalFormat(m_Specification.Format), m_Specification.Width, m_Specification.Height);
		for (int i = 0; i < 6; ++i) {
			glTextureSubImage3D(m_TextureID, 0, 0, 0, i,
				m_Specification.Width, m_Specification.Height, 1,
				ImageUtils::ImageFormatToOpenGLFormat(m_Specification.Format),
				ImageUtils::ImageFormatToGLByteType(m_Specification.Format), m_ImageDatas[i].Data);
		}

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, ImageUtils::ImageFilterToGL(m_Specification.SamplerFilter));
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, ImageUtils::ImageFilterToGL(m_Specification.SamplerFilter));
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, ImageUtils::ImageWrapToGL(m_Specification.SamplerWrap));
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, ImageUtils::ImageWrapToGL(m_Specification.SamplerWrap));
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_R, ImageUtils::ImageWrapToGL(m_Specification.SamplerWrap));
		});
}

void Nut::TextureCube::Bind(uint32_t slot)
{
}

void Nut::TextureCube::Unbind()
{
}

void Nut::TextureCube::SetData(const void* data, uint32_t size, uint32_t faceindex)
{
	if (faceindex > 6) {
		NUT_ERROR_TAG("Texture", "立方体纹理的面索引超出范围: {0}", faceindex);
		return;
	}

	m_ImageDatas[faceindex].Release();
	m_ImageDatas[faceindex].Allocate(size);
	m_ImageDatas[faceindex].Write(data, size, 0);

	if (m_TextureID) {
		Renderer::Submit([this, faceindex]() {
			glTextureSubImage3D(m_TextureID, 0, 0, 0, faceindex,
				m_Specification.Width, m_Specification.Height, 1,
				ImageUtils::ImageFormatToOpenGLFormat(m_Specification.Format),
				ImageUtils::ImageFormatToGLByteType(m_Specification.Format), m_ImageDatas[faceindex].Data);
			});
	}
	else {
		Invalidate();
	}
}

std::shared_ptr<Nut::TextureCube> Nut::TextureCube::Create(const TextureSpecification& spec)
{
	std::shared_ptr<TextureCube> texture = std::make_shared<TextureCube>(spec);
	s_Textures.push_back(texture);
	return texture;
}

std::shared_ptr<Nut::TextureCube> Nut::TextureCube::Create(const TextureSpecification& spec, std::vector<Buffer> imageDatas)
{
	std::shared_ptr<TextureCube> texture = std::make_shared<TextureCube>(spec, imageDatas);
	s_Textures.push_back(texture);
	return texture;
}

std::shared_ptr<Nut::TextureCube> Nut::TextureCube::Create(const TextureSpecification& spec, const std::filesystem::path& filepath)
{
	std::shared_ptr<TextureCube> texture = std::make_shared<TextureCube>(spec, filepath);
	s_Textures.push_back(texture);
	return texture;
}

