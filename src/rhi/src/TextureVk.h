#pragma once

#include <vulkan/vulkan.h>

#include "rhi/Texture.h"
#include "ResourceVk.h"
#include "ContextVk.h"

namespace rhi
{




	struct FormatInfo
	{
		TextureFormat format;
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

	class TextureVk final : public ITexture, public MemoryResource
	{
	public:
		TextureVk(const ContextVk& context, const VmaAllocator& allocator)
			:m_Context(context),
			m_Allocator(allocator) {}
		~TextureVk();
		TextureDesc desc;
		VkImage image = VK_NULL_HANDLE;
		VkImageView	view = VK_NULL_HANDLE;
		VkFormat format = VK_FORMAT_UNDEFINED;
	private:
		TextureVk() = default;
		const ContextVk& m_Context;
		const VmaAllocator& m_Allocator;
	};

	const FormatInfo& getFormatInfo(TextureFormat format);

	VkImageUsageFlags getVkImageUsageFlags(const TextureDesc& desc);

	VkImageCreateFlags getVkImageCreateFlags(TextureDimension dimension);

	VkSampleCountFlagBits getVkImageSampleCount(const TextureDesc& desc);

	VkImageType getVkImageType(TextureDimension dimension);

	VkImageViewType getVkImageViewType(TextureDimension dimension);

	VkImageAspectFlags getVkAspectMask(VkFormat format);

	VkFormat textureFormatToVkFormat(TextureFormat format);

	TextureFormat vkFormatToTextureFormat(VkFormat format);
}