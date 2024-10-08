#include "Texture.h"


namespace rhi
{
	VkFormat convertFormat(Format format)
	{
		assert(format < Format::COUNT);
		assert(c_FormatMap[uint32_t(format)].rhiFormat == format);

		return c_FormatMap[uint32_t(format)].vkFormat;
	}

	VkImageAspectFlags getAspectMask(VkFormat format)
	{
		switch (format)
		{
		case VK_FORMAT_S8_UINT:
			return VK_IMAGE_ASPECT_STENCIL_BIT;
			break;
		case VK_FORMAT_D16_UNORM:
		case VK_FORMAT_X8_D24_UNORM_PACK32:
		case VK_FORMAT_D32_SFLOAT:
			return VK_IMAGE_ASPECT_DEPTH_BIT;
		case VK_FORMAT_D16_UNORM_S8_UINT:
		case VK_FORMAT_D24_UNORM_S8_UINT:
		case VK_FORMAT_D32_SFLOAT_S8_UINT:
			return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		default:
			return VK_IMAGE_ASPECT_COLOR_BIT;
		}
	}
}