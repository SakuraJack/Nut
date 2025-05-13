#pragma once

#include "Renderer/Image.h"
#include "glad/glad.h"

namespace Nut {

	namespace ImageUtils
	{
		static unsigned int ImageFormatDataSize(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RED8:			return 1;
			case ImageFormat::RED16:		return 2;
			case ImageFormat::RED32:		return 4;
			case ImageFormat::RED32F:		return 4;
			case ImageFormat::RG8:			return 2;
			case ImageFormat::RG16F:		return 4;
			case ImageFormat::RG32F:		return 8;
			case ImageFormat::RGB:			return 3;
			case ImageFormat::RGBA:			return 4;
			case ImageFormat::RGBA16F:		return 8;
			case ImageFormat::RGBA32F:		return 16;
			case ImageFormat::SRGB:			return 4;
			case ImageFormat::DEPTH32FSTENCIL8UINT: return 8;
			case ImageFormat::DEPTH32F:		return 4;
			case ImageFormat::DEPTH24STENCIL8: return 4;
			}
		}

		static GLenum ImageWrapToGL(TextureWrap wrap)
		{
			switch (wrap)
			{
			case TextureWrap::Clamp:		return GL_CLAMP_TO_EDGE;
			case TextureWrap::Repeat:		return GL_REPEAT;
			}
		}

		static GLenum ImageFilterToGL(TextureFilter filter)
		{
			switch (filter)
			{
			case TextureFilter::Linear:		return GL_LINEAR;
			case TextureFilter::Nearest:	return GL_NEAREST;
			case TextureFilter::Cubic:		return GL_LINEAR;
			}
		}

		static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RED8:			return GL_R8;
			case ImageFormat::RED16:		return GL_R16;
			case ImageFormat::RED32:		return GL_R32I;
			case ImageFormat::RED32F:	return GL_R32F;
			case ImageFormat::RG8:		return GL_RG8;
			case ImageFormat::RG16F:	return GL_RG16F;
			case ImageFormat::RG32F:	return GL_RG32F;
			case ImageFormat::RGB:		return GL_RGB;
			case ImageFormat::RGBA:		return GL_RGBA;
			case ImageFormat::RGBA16F:	return GL_RGBA16F;
			case ImageFormat::RGBA32F:	return GL_RGBA32F;
			case ImageFormat::SRGB:		return GL_SRGB;
			case ImageFormat::DEPTH32FSTENCIL8UINT: return GL_DEPTH32F_STENCIL8;
			case ImageFormat::DEPTH32F:		return GL_DEPTH_COMPONENT32F;
			case ImageFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
			}
		}

		static GLenum ImageFormatToOpenGLFormat(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RED8:			return GL_RED_INTEGER;
			case ImageFormat::RED16:		return GL_RED_INTEGER;
			case ImageFormat::RED32:		return GL_RED_INTEGER;
			case ImageFormat::RED32F:		return GL_RED;
			case ImageFormat::RG8:			return GL_RG;
			case ImageFormat::RG16F:		return GL_RG16F;
			case ImageFormat::RG32F:		return GL_RG32F;
			case ImageFormat::RGB:			return GL_RGB;
			case ImageFormat::RGBA:			return GL_RGBA;
			case ImageFormat::RGBA16F:		return GL_RGBA16F;
			case ImageFormat::RGBA32F:		return GL_RGBA32F;
			case ImageFormat::SRGB:			return GL_SRGB;
			case ImageFormat::DEPTH32FSTENCIL8UINT: return GL_DEPTH_STENCIL;
			case ImageFormat::DEPTH32F:		return GL_DEPTH_COMPONENT;
			case ImageFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL;
			}
		}

		static GLenum ImageFormatToGLByteType(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RED8:			return GL_UNSIGNED_BYTE;
			case ImageFormat::RED16:		return GL_UNSIGNED_SHORT;
			case ImageFormat::RED32:		return GL_INT;
			case ImageFormat::RED32F:	return GL_FLOAT;
			case ImageFormat::RG8:		return GL_UNSIGNED_BYTE;
			case ImageFormat::RG16F:	return GL_HALF_FLOAT;
			case ImageFormat::RG32F:	return GL_FLOAT;
			case ImageFormat::RGB:		return GL_UNSIGNED_BYTE;
			case ImageFormat::RGBA:		return GL_UNSIGNED_BYTE;
			case ImageFormat::RGBA16F:	return GL_HALF_FLOAT;
			case ImageFormat::RGBA32F:	return GL_FLOAT;
			case ImageFormat::SRGB:		return GL_UNSIGNED_BYTE;
			case ImageFormat::DEPTH32FSTENCIL8UINT: return GL_UNSIGNED_INT_24_8;
			case ImageFormat::DEPTH32F:		return GL_FLOAT;
			case ImageFormat::DEPTH24STENCIL8: return GL_UNSIGNED_INT_24_8;
			}
		}
	}
}

