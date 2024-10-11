#pragma once

#include "ITexture.h"

#include <vulkan/vulkan.h>

namespace rhi
{
	struct FormatMapping
	{
		Format rhiFormat;
		VkFormat vkFormat;
	};

	struct FormatInfo
	{
		Format format;
		const char* name;
		uint8_t bytesPerBlock;
		uint8_t blockSize;
		bool hasRed : 1;
		bool hasGreen : 1;
		bool hasBlue : 1;
		bool hasAlpha : 1;
		bool hasDepth : 1;
		bool hasStencil : 1;
	};

	class Texture : public ITexture
	{

	};
}