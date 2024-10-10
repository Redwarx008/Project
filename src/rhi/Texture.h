#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "RHI.h"
#include "VkContext.h"

#include <array>

namespace rhi
{

	enum class TextureDimension : uint8_t
	{
		Unknown,
		Texture1D,
		Texture1DArray,
		Texture2D,
		Texture2DArray,
		TextureCube,
		TextureCubeArray,
		Texture3D
	};

	enum class Format : uint8_t
	{
		UNKNOWN,

		R8_UINT,
		R8_SINT,
		R8_UNORM,
		R8_SNORM,
		RG8_UINT,
		RG8_SINT,
		RG8_UNORM,
		RG8_SNORM,
		R16_UINT,
		R16_SINT,
		R16_UNORM,
		R16_SNORM,
		R16_FLOAT,
		BGRA4_UNORM,
		B5G6R5_UNORM,
		B5G5R5A1_UNORM,
		RGBA8_UINT,
		RGBA8_SINT,
		RGBA8_UNORM,
		RGBA8_SNORM,
		BGRA8_UNORM,
		SRGBA8_UNORM,
		SBGRA8_UNORM,
		R10G10B10A2_UNORM,
		R11G11B10_FLOAT,
		RG16_UINT,
		RG16_SINT,
		RG16_UNORM,
		RG16_SNORM,
		RG16_FLOAT,
		R32_UINT,
		R32_SINT,
		R32_FLOAT,
		RGBA16_UINT,
		RGBA16_SINT,
		RGBA16_FLOAT,
		RGBA16_UNORM,
		RGBA16_SNORM,
		RG32_UINT,
		RG32_SINT,
		RG32_FLOAT,
		RGB32_UINT,
		RGB32_SINT,
		RGB32_FLOAT,
		RGBA32_UINT,
		RGBA32_SINT,
		RGBA32_FLOAT,

		D16,
		D24S8,
		X24G8_UINT,
		D32,
		D32S8,
		X32G8_UINT,

		BC1_UNORM,
		BC1_UNORM_SRGB,
		BC2_UNORM,
		BC2_UNORM_SRGB,
		BC3_UNORM,
		BC3_UNORM_SRGB,
		BC4_UNORM,
		BC4_SNORM,
		BC5_UNORM,
		BC5_SNORM,
		BC6H_UFLOAT,
		BC6H_SFLOAT,
		BC7_UNORM,
		BC7_UNORM_SRGB,

		COUNT,
	};

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

	struct TextureDesc
	{
		TextureDimension dimension = TextureDimension::Unknown;
		uint32_t width = 1;
		uint32_t height = 1;
		/// For a 1D array or 2D array, number of array slices
		uint32_t arraySize = 1;
		/// For a 3D texture, number of depth slices
		uint32_t depth = 1;
		uint32_t sampleCount = 1;
		uint32_t mipLevels = 1;
		Format format = Format::UNKNOWN;

		bool isSampled = true;
		bool isStorage = false;
		bool isRenderTarget = false;

		ResourceStates initialState = ResourceStates::Unknown;

		constexpr TextureDesc& setWidth(uint32_t value) { width = value; return *this; }
		constexpr TextureDesc& setHeight(uint32_t value) { height = value; return *this; }
		constexpr TextureDesc& setDepth(uint32_t value) { depth = value; return *this; }
		constexpr TextureDesc& setArraySize(uint32_t value) { arraySize = value; return *this; }
		constexpr TextureDesc& setMipLevels(uint32_t value) { mipLevels = value; return *this; }
		constexpr TextureDesc& setFormat(Format value) { format = value; return *this; }
		constexpr TextureDesc& setDimension(TextureDimension value) { dimension = value; return *this; }
		constexpr TextureDesc& setInitialState(ResourceStates value) { initialState = value; return *this; }
		constexpr TextureDesc& setIsSampled(bool value) { isSampled = value; return *this; }
		constexpr TextureDesc& setIsStorage(bool value) { isStorage = value; return *this; }
		constexpr TextureDesc& setIsRenderTarget(bool value) { isRenderTarget = value; return *this; }
	};

	class Texture : public MemoryResource
	{
	public:
		Texture(const VkContext& context, const VmaAllocator& allocator)
			:m_Context(context),
			m_Allocator(allocator)
		{}
		Texture() = delete;
		~Texture();
		TextureDesc desc;
		VkImage image;
		VkImageView	view;
		VkFormat format;

	private:
		const VkContext& m_Context;
		const VmaAllocator& m_Allocator;
	};

	const FormatInfo& GetFormatInfo(Format format);

	VkImageUsageFlags GetVkImageUsageFlags(const TextureDesc& desc);

	VkImageCreateFlags GetVkImageCreateFlags(TextureDimension dimension);

	VkSampleCountFlagBits GetVkImageSampleCount(const TextureDesc& desc);

	VkImageType GetVkImageType(TextureDimension dimension);

	VkImageViewType GetVkImageViewType(TextureDimension dimension);

	VkFormat GetVkFormat(Format format);

	VkImageAspectFlags GetVkAspectMask(VkFormat format);
}