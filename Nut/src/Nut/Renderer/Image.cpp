#include "ntpch.h"
#include "Image.h"
#include "Nut/Utils/ImageUtils.h"

std::shared_ptr<Nut::Image2D> Nut::Image2D::Create(const ImageSpecification& spec, Buffer buffer)
{
	return std::make_shared<Image2D>(spec, buffer);
}

std::shared_ptr<Nut::Image2D> Nut::Image2D::Create(const ImageSpecification& spec, const void* data /*= nullptr*/)
{
	return std::make_shared<Image2D>(spec, data);
}

void Nut::Image2D::Resize(const glm::uvec2& size)
{
	Resize(size.x, size.y);
}

void Nut::Image2D::Resize(const uint32_t width, const uint32_t height)
{
	if (width == 0 || height == 0) return;
	if (width == m_Specification.Width && height == m_Specification.Height) return;
	m_Specification.Width = width;
	m_Specification.Height = height;
	Invalidate();
}

void Nut::Image2D::Invalidate()
{
	if (m_ImageData.Data)
	{
		m_ImageData.Release();
	}

	m_ImageData.Allocate(m_Specification.Width * m_Specification.Height * ImageUtils::ImageFormatDataSize(m_Specification.Format));
	m_ImageData.Zero();
}

void Nut::Image2D::Release()
{
	if (m_ImageData.Data)
	{
		m_ImageData.Release();
	}
}

Nut::Image2D::Image2D(const ImageSpecification& spec, Buffer buffer)
	: m_Specification(spec), m_ImageData(buffer)
{

}

Nut::Image2D::~Image2D()
{
	Release();
}

Nut::Image2D::Image2D(const ImageSpecification& spec, const void* data /*= nullptr*/)
	: m_Specification(spec), m_ImageData(data)
{

}
