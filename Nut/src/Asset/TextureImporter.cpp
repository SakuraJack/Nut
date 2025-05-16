#include "ntpch.h"
#include "TextureImporter.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Nut::Buffer Nut::TextureImporter::ToBufferFromFile(const std::filesystem::path& path, ImageFormat& outFormat, uint32_t& outWidth, uint32_t& outHeight)
{
	Buffer imageBuffer;
	std::string pathString = path.string();
	stbi_set_flip_vertically_on_load(0);

	int width, height, channels;
	if (stbi_is_hdr(pathString.c_str()))
	{
		imageBuffer.Data = (byte*)stbi_loadf(pathString.c_str(), &width, &height, &channels, 4);
		imageBuffer.Size = width * height * 4 * sizeof(float);
		outFormat = ImageFormat::RGBA32F;
	}
	else {
		imageBuffer.Data = (byte*)stbi_load(pathString.c_str(), &width, &height, &channels, 4);
		imageBuffer.Size = width * height * 4;
		outFormat = ImageFormat::RGBA;
	}

	if (!imageBuffer.Data) {
		return {};
	}

	outWidth = width;
	outHeight = height;
	return imageBuffer;
}

Nut::Buffer Nut::TextureImporter::ToBufferFromMemory(Buffer buffer, ImageFormat& outFormat, uint32_t& outWidth, uint32_t& outHeight)
{
	Buffer imageBuffer;
	stbi_set_flip_vertically_on_load(0);

	int width, height, channels;
	if (stbi_is_hdr_from_memory((const stbi_uc*)buffer.Data, (int)buffer.Size)) {
		imageBuffer.Data = (byte*)stbi_loadf_from_memory((const stbi_uc*)buffer.Data, (int)buffer.Size, &width, &height, &channels, STBI_rgb_alpha);
		imageBuffer.Size = width * height * 4 * sizeof(float);
		outFormat = ImageFormat::RGBA32F;
	}
	else {
		imageBuffer.Data = (byte*)stbi_load_from_memory((const stbi_uc*)buffer.Data, (int)buffer.Size, &width, &height, &channels, STBI_rgb_alpha);
		imageBuffer.Size = width * height * 4;
		outFormat = ImageFormat::RGBA;
	}

	if (!imageBuffer.Data) {
		return {};
	}

	outWidth = width;
	outHeight = height;
	return imageBuffer;
}
