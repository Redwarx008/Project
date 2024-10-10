#include "Texture.h"

#include <assert.h>

namespace rhi
{
	static const std::array<FormatMapping, size_t(Format::COUNT)> c_FormatMap = { {
	{ Format::UNKNOWN,           VK_FORMAT_UNDEFINED                },
	{ Format::R8_UINT,           VK_FORMAT_R8_UINT                  },
	{ Format::R8_SINT,           VK_FORMAT_R8_SINT                  },
	{ Format::R8_UNORM,          VK_FORMAT_R8_UNORM                 },
	{ Format::R8_SNORM,          VK_FORMAT_R8_SNORM                 },
	{ Format::RG8_UINT,          VK_FORMAT_R8G8_UINT                },
	{ Format::RG8_SINT,          VK_FORMAT_R8G8_SINT                },
	{ Format::RG8_UNORM,         VK_FORMAT_R8G8_UNORM               },
	{ Format::RG8_SNORM,         VK_FORMAT_R8G8_SNORM               },
	{ Format::R16_UINT,          VK_FORMAT_R16_UINT                 },
	{ Format::R16_SINT,          VK_FORMAT_R16_SINT                 },
	{ Format::R16_UNORM,         VK_FORMAT_R16_UNORM                },
	{ Format::R16_SNORM,         VK_FORMAT_R16_SNORM                },
	{ Format::R16_FLOAT,         VK_FORMAT_R16_SFLOAT               },
	{ Format::BGRA4_UNORM,       VK_FORMAT_B4G4R4A4_UNORM_PACK16    },
	{ Format::B5G6R5_UNORM,      VK_FORMAT_B5G6R5_UNORM_PACK16      },
	{ Format::B5G5R5A1_UNORM,    VK_FORMAT_B5G5R5A1_UNORM_PACK16    },
	{ Format::RGBA8_UINT,        VK_FORMAT_R8G8B8A8_UINT            },
	{ Format::RGBA8_SINT,        VK_FORMAT_R8G8B8A8_SINT            },
	{ Format::RGBA8_UNORM,       VK_FORMAT_R8G8B8A8_UNORM           },
	{ Format::RGBA8_SNORM,       VK_FORMAT_R8G8B8A8_SNORM           },
	{ Format::BGRA8_UNORM,       VK_FORMAT_B8G8R8A8_UNORM           },
	{ Format::SRGBA8_UNORM,      VK_FORMAT_R8G8B8A8_SRGB            },
	{ Format::SBGRA8_UNORM,      VK_FORMAT_B8G8R8A8_SRGB            },
	{ Format::R10G10B10A2_UNORM, VK_FORMAT_A2B10G10R10_UNORM_PACK32 },
	{ Format::R11G11B10_FLOAT,   VK_FORMAT_B10G11R11_UFLOAT_PACK32  },
	{ Format::RG16_UINT,         VK_FORMAT_R16G16_UINT              },
	{ Format::RG16_SINT,         VK_FORMAT_R16G16_SINT              },
	{ Format::RG16_UNORM,        VK_FORMAT_R16G16_UNORM             },
	{ Format::RG16_SNORM,        VK_FORMAT_R16G16_SNORM             },
	{ Format::RG16_FLOAT,        VK_FORMAT_R16G16_SFLOAT            },
	{ Format::R32_UINT,          VK_FORMAT_R32_UINT                 },
	{ Format::R32_SINT,          VK_FORMAT_R32_SINT                 },
	{ Format::R32_FLOAT,         VK_FORMAT_R32_SFLOAT               },
	{ Format::RGBA16_UINT,       VK_FORMAT_R16G16B16A16_UINT        },
	{ Format::RGBA16_SINT,       VK_FORMAT_R16G16B16A16_SINT        },
	{ Format::RGBA16_FLOAT,      VK_FORMAT_R16G16B16A16_SFLOAT      },
	{ Format::RGBA16_UNORM,      VK_FORMAT_R16G16B16A16_UNORM       },
	{ Format::RGBA16_SNORM,      VK_FORMAT_R16G16B16A16_SNORM       },
	{ Format::RG32_UINT,         VK_FORMAT_R32G32_UINT              },
	{ Format::RG32_SINT,         VK_FORMAT_R32G32_SINT              },
	{ Format::RG32_FLOAT,        VK_FORMAT_R32G32_SFLOAT            },
	{ Format::RGB32_UINT,        VK_FORMAT_R32G32B32_UINT           },
	{ Format::RGB32_SINT,        VK_FORMAT_R32G32B32_SINT           },
	{ Format::RGB32_FLOAT,       VK_FORMAT_R32G32B32_SFLOAT         },
	{ Format::RGBA32_UINT,       VK_FORMAT_R32G32B32A32_UINT        },
	{ Format::RGBA32_SINT,       VK_FORMAT_R32G32B32A32_SINT        },
	{ Format::RGBA32_FLOAT,      VK_FORMAT_R32G32B32A32_SFLOAT      },
	{ Format::D16,               VK_FORMAT_D16_UNORM                },
	{ Format::D24S8,             VK_FORMAT_D24_UNORM_S8_UINT        },
	{ Format::X24G8_UINT,        VK_FORMAT_D24_UNORM_S8_UINT        },
	{ Format::D32,               VK_FORMAT_D32_SFLOAT               },
	{ Format::D32S8,             VK_FORMAT_D32_SFLOAT_S8_UINT       },
	{ Format::X32G8_UINT,        VK_FORMAT_D32_SFLOAT_S8_UINT       },
	{ Format::BC1_UNORM,         VK_FORMAT_BC1_RGBA_UNORM_BLOCK     },
	{ Format::BC1_UNORM_SRGB,    VK_FORMAT_BC1_RGBA_SRGB_BLOCK      },
	{ Format::BC2_UNORM,         VK_FORMAT_BC2_UNORM_BLOCK          },
	{ Format::BC2_UNORM_SRGB,    VK_FORMAT_BC2_SRGB_BLOCK           },
	{ Format::BC3_UNORM,         VK_FORMAT_BC3_UNORM_BLOCK          },
	{ Format::BC3_UNORM_SRGB,    VK_FORMAT_BC3_SRGB_BLOCK           },
	{ Format::BC4_UNORM,         VK_FORMAT_BC4_UNORM_BLOCK          },
	{ Format::BC4_SNORM,         VK_FORMAT_BC4_SNORM_BLOCK          },
	{ Format::BC5_UNORM,         VK_FORMAT_BC5_UNORM_BLOCK          },
	{ Format::BC5_SNORM,         VK_FORMAT_BC5_SNORM_BLOCK          },
	{ Format::BC6H_UFLOAT,       VK_FORMAT_BC6H_UFLOAT_BLOCK        },
	{ Format::BC6H_SFLOAT,       VK_FORMAT_BC6H_SFLOAT_BLOCK        },
	{ Format::BC7_UNORM,         VK_FORMAT_BC7_UNORM_BLOCK          },
	{ Format::BC7_UNORM_SRGB,    VK_FORMAT_BC7_SRGB_BLOCK           },

	} };

	    // Format mapping table. The rows must be in the exactly same order as Format enum members are defined.
    static const FormatInfo c_FormatInfo[] = {
    //        format                   name             bytes blk   red   green   blue  alpha  depth  stencl 
        { Format::UNKNOWN,           "UNKNOWN",           0,   0,  false, false, false, false, false, false,  },
        { Format::R8_UINT,           "R8_UINT",           1,   1,  true,  false, false, false, false, false,  },
        { Format::R8_SINT,           "R8_SINT",           1,   1,  true,  false, false, false, false, false,  },
        { Format::R8_UNORM,          "R8_UNORM",          1,   1,  true,  false, false, false, false, false,  },
        { Format::R8_SNORM,          "R8_SNORM",          1,   1,  true,  false, false, false, false, false,  },
        { Format::RG8_UINT,          "RG8_UINT",          2,   1,  true,  true,  false, false, false, false,  },
        { Format::RG8_SINT,          "RG8_SINT",          2,   1,  true,  true,  false, false, false, false,  },
        { Format::RG8_UNORM,         "RG8_UNORM",         2,   1,  true,  true,  false, false, false, false,  },
        { Format::RG8_SNORM,         "RG8_SNORM",         2,   1,  true,  true,  false, false, false, false,  },
        { Format::R16_UINT,          "R16_UINT",          2,   1,  true,  false, false, false, false, false,  },
        { Format::R16_SINT,          "R16_SINT",          2,   1,  true,  false, false, false, false, false,  },
        { Format::R16_UNORM,         "R16_UNORM",         2,   1,  true,  false, false, false, false, false,  },
        { Format::R16_SNORM,         "R16_SNORM",         2,   1,  true,  false, false, false, false, false,  },
        { Format::R16_FLOAT,         "R16_FLOAT",         2,   1,  true,  false, false, false, false, false,  },
        { Format::BGRA4_UNORM,       "BGRA4_UNORM",       2,   1,  true,  true,  true,  true,  false, false,  },
        { Format::B5G6R5_UNORM,      "B5G6R5_UNORM",      2,   1,  true,  true,  true,  false, false, false,  },
        { Format::B5G5R5A1_UNORM,    "B5G5R5A1_UNORM",    2,   1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA8_UINT,        "RGBA8_UINT",        4,   1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA8_SINT,        "RGBA8_SINT",        4,   1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA8_UNORM,       "RGBA8_UNORM",       4,   1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA8_SNORM,       "RGBA8_SNORM",       4,   1,  true,  true,  true,  true,  false, false,  },
        { Format::BGRA8_UNORM,       "BGRA8_UNORM",       4,   1,  true,  true,  true,  true,  false, false,  },
        { Format::SRGBA8_UNORM,      "SRGBA8_UNORM",      4,   1,  true,  true,  true,  true,  false, false,  },
        { Format::SBGRA8_UNORM,      "SBGRA8_UNORM",      4,   1,  true,  true,  true,  true,  false, false,  },
        { Format::R10G10B10A2_UNORM, "R10G10B10A2_UNORM", 4,   1,  true,  true,  true,  true,  false, false,  },
        { Format::R11G11B10_FLOAT,   "R11G11B10_FLOAT",   4,   1,  true,  true,  true,  false, false, false,  },
        { Format::RG16_UINT,         "RG16_UINT",         4,   1,  true,  true,  false, false, false, false,  },
        { Format::RG16_SINT,         "RG16_SINT",         4,   1,  true,  true,  false, false, false, false,  },
        { Format::RG16_UNORM,        "RG16_UNORM",        4,   1,  true,  true,  false, false, false, false,  },
        { Format::RG16_SNORM,        "RG16_SNORM",        4,   1,  true,  true,  false, false, false, false,  },
        { Format::RG16_FLOAT,        "RG16_FLOAT",        4,   1,  true,  true,  false, false, false, false,  },
        { Format::R32_UINT,          "R32_UINT",          4,   1,  true,  false, false, false, false, false,  },
        { Format::R32_SINT,          "R32_SINT",          4,   1,  true,  false, false, false, false, false,  },
        { Format::R32_FLOAT,         "R32_FLOAT",         4,   1,  true,  false, false, false, false, false,  },
        { Format::RGBA16_UINT,       "RGBA16_UINT",       8,   1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA16_SINT,       "RGBA16_SINT",       8,   1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA16_FLOAT,      "RGBA16_FLOAT",      8,   1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA16_UNORM,      "RGBA16_UNORM",      8,   1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA16_SNORM,      "RGBA16_SNORM",      8,   1,  true,  true,  true,  true,  false, false,  },
        { Format::RG32_UINT,         "RG32_UINT",         8,   1,  true,  true,  false, false, false, false,  },
        { Format::RG32_SINT,         "RG32_SINT",         8,   1,  true,  true,  false, false, false, false,  },
        { Format::RG32_FLOAT,        "RG32_FLOAT",        8,   1,  true,  true,  false, false, false, false,  },
        { Format::RGB32_UINT,        "RGB32_UINT",        12,  1,  true,  true,  true,  false, false, false,  },
        { Format::RGB32_SINT,        "RGB32_SINT",        12,  1,  true,  true,  true,  false, false, false,  },
        { Format::RGB32_FLOAT,       "RGB32_FLOAT",       12,  1,  true,  true,  true,  false, false, false,  },
        { Format::RGBA32_UINT,       "RGBA32_UINT",       16,  1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA32_SINT,       "RGBA32_SINT",       16,  1,  true,  true,  true,  true,  false, false,  },
        { Format::RGBA32_FLOAT,      "RGBA32_FLOAT",      16,  1,  true,  true,  true,  true,  false, false,  },
        { Format::D16,               "D16",               2,   1,  false, false, false, false, true,  false,  },
        { Format::D24S8,             "D24S8",             4,   1,  false, false, false, false, true,  true,   },
        { Format::X24G8_UINT,        "X24G8_UINT",        4,   1,  false, false, false, false, false, true,   },
        { Format::D32,               "D32",               4,   1,  false, false, false, false, true,  false,  },
        { Format::D32S8,             "D32S8",             8,   1,  false, false, false, false, true,  true,   },
        { Format::X32G8_UINT,        "X32G8_UINT",        8,   1,  false, false, false, false, false, true,   },
        { Format::BC1_UNORM,         "BC1_UNORM",         8,   4,  true,  true,  true,  true,  false, false,  },
        { Format::BC1_UNORM_SRGB,    "BC1_UNORM_SRGB",    8,   4,  true,  true,  true,  true,  false, false,  },
        { Format::BC2_UNORM,         "BC2_UNORM",         16,  4,  true,  true,  true,  true,  false, false,  },
        { Format::BC2_UNORM_SRGB,    "BC2_UNORM_SRGB",    16,  4,  true,  true,  true,  true,  false, false,  },
        { Format::BC3_UNORM,         "BC3_UNORM",         16,  4,  true,  true,  true,  true,  false, false,  },
        { Format::BC3_UNORM_SRGB,    "BC3_UNORM_SRGB",    16,  4,  true,  true,  true,  true,  false, false,  },
        { Format::BC4_UNORM,         "BC4_UNORM",         8,   4,  true,  false, false, false, false, false,  },
        { Format::BC4_SNORM,         "BC4_SNORM",         8,   4,  true,  false, false, false, false, false,  },
        { Format::BC5_UNORM,         "BC5_UNORM",         16,  4,  true,  true,  false, false, false, false,  },
        { Format::BC5_SNORM,         "BC5_SNORM",         16,  4,  true,  true,  false, false, false, false,  },
        { Format::BC6H_UFLOAT,       "BC6H_UFLOAT",       16,  4,  true,  true,  true,  false, false, false,  },
        { Format::BC6H_SFLOAT,       "BC6H_SFLOAT",       16,  4,  true,  true,  true,  false, false, false,  },
        { Format::BC7_UNORM,         "BC7_UNORM",         16,  4,  true,  true,  true,  true,  false, false,  },
        { Format::BC7_UNORM_SRGB,    "BC7_UNORM_SRGB",    16,  4,  true,  true,  true,  true,  false, false,  },
    };
	const FormatInfo& GetFormatInfo(Format format)
	{
		static_assert(sizeof(c_FormatInfo) / sizeof(FormatInfo) == size_t(Format::COUNT),
			"The format info table doesn't have the right number of elements");
		const FormatInfo& info = c_FormatInfo[uint32_t(format)];
		assert(info.format == format);
		return info;
	}

	VkImageUsageFlags GetVkImageUsageFlags(const TextureDesc& desc)
	{
		const FormatInfo& formatInfo = GetFormatInfo(desc.format);

		VkImageUsageFlags flags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		if (desc.isSampled)
		{
			flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
		}

		if (desc.isStorage)
		{
			flags |= VK_IMAGE_USAGE_STORAGE_BIT;
		}

		if (desc.isRenderTarget)
		{
			if (formatInfo.hasDepth || formatInfo.hasStencil)
			{
				flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			}
			else
			{
				flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			}
		}
		return flags;
	}

	VkImageCreateFlags GetVkImageCreateFlags(TextureDimension dimension)
	{
		VkImageCreateFlags flags = 0;
		if (dimension == TextureDimension::TextureCube ||
			dimension == TextureDimension::TextureCubeArray)
		{
			flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		}
		return flags;
	}

	VkSampleCountFlagBits GetVkImageSampleCount(const TextureDesc& desc)
	{
		switch (desc.sampleCount)
		{
		case 1:
			return VK_SAMPLE_COUNT_1_BIT;

		case 2:
			return VK_SAMPLE_COUNT_2_BIT;

		case 4:
			return VK_SAMPLE_COUNT_4_BIT;

		case 8:
			return VK_SAMPLE_COUNT_8_BIT;

		case 16:
			return VK_SAMPLE_COUNT_16_BIT;

		case 32:
			return VK_SAMPLE_COUNT_32_BIT;

		case 64:
			return VK_SAMPLE_COUNT_64_BIT;

		default:
			assert(!"Invalid Enumeration Value");
			return VK_SAMPLE_COUNT_1_BIT;
		}
	}

	VkImageType GetVkImageType(TextureDimension dimension)
	{
		switch (dimension)
		{
		case TextureDimension::Texture1D:
		case TextureDimension::Texture1DArray:
			return VK_IMAGE_TYPE_1D;

		case TextureDimension::Texture2D:
		case TextureDimension::Texture2DArray:
		case TextureDimension::TextureCube:
		case TextureDimension::TextureCubeArray:
			return VK_IMAGE_TYPE_2D;

		case TextureDimension::Texture3D:
			return VK_IMAGE_TYPE_2D;

		case TextureDimension::Unknown:
		default:
			assert(!"Invalid Enumeration Value");
		}
	}

	VkImageViewType GetVkImageViewType(TextureDimension dimension)
	{
		switch (dimension)
		{
		case TextureDimension::Texture1D:
			return VK_IMAGE_VIEW_TYPE_1D;

		case TextureDimension::Texture1DArray:
			return VK_IMAGE_VIEW_TYPE_1D_ARRAY;

		case TextureDimension::Texture2D:
			return VK_IMAGE_VIEW_TYPE_2D;

		case TextureDimension::Texture2DArray:
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;

		case TextureDimension::TextureCube:
			return VK_IMAGE_VIEW_TYPE_CUBE;

		case TextureDimension::TextureCubeArray:
			return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;

		case TextureDimension::Texture3D:
			return VK_IMAGE_VIEW_TYPE_3D;

		case TextureDimension::Unknown:
		default:
			assert(!"Invalid Enumeration Value");
			return VK_IMAGE_VIEW_TYPE_2D;
		}
	}

	VkFormat GetVkFormat(Format format)
	{
		assert(format < Format::COUNT);
		assert(c_FormatMap[uint32_t(format)].rhiFormat == format);

		return c_FormatMap[uint32_t(format)].vkFormat;
	}

	VkImageAspectFlags GetVkAspectMask(VkFormat format)
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

	Texture::~Texture()
	{
		if (view)
		{
			vkDestroyImageView(m_Context.device, view, nullptr);
			view = VK_NULL_HANDLE;
		}
		if (managed)
		{
			vmaDestroyImage(m_Allocator, image, allocation);
			image = VK_NULL_HANDLE;
		}
	}

}