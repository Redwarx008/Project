#include "TextureVk.h"

#include <array>
#include <unordered_map>
#include <cassert>
namespace rhi
{
	struct TexFmtToVkFmtMapping
	{
		TextureFormat rhiFormat;
		VkFormat vkFormat;
	};

	static const std::array<TexFmtToVkFmtMapping, size_t(TextureFormat::COUNT)> _texFmtToVkFmtMap = { {
		{ TextureFormat::UNKNOWN,           VK_FORMAT_UNDEFINED                },
		{ TextureFormat::R8_UINT,           VK_FORMAT_R8_UINT                  },
		{ TextureFormat::R8_SINT,           VK_FORMAT_R8_SINT                  },
		{ TextureFormat::R8_UNORM,          VK_FORMAT_R8_UNORM                 },
		{ TextureFormat::R8_SNORM,          VK_FORMAT_R8_SNORM                 },
		{ TextureFormat::RG8_UINT,          VK_FORMAT_R8G8_UINT                },
		{ TextureFormat::RG8_SINT,          VK_FORMAT_R8G8_SINT                },
		{ TextureFormat::RG8_UNORM,         VK_FORMAT_R8G8_UNORM               },
		{ TextureFormat::RG8_SNORM,         VK_FORMAT_R8G8_SNORM               },
		{ TextureFormat::R16_UINT,          VK_FORMAT_R16_UINT                 },
		{ TextureFormat::R16_SINT,          VK_FORMAT_R16_SINT                 },
		{ TextureFormat::R16_UNORM,         VK_FORMAT_R16_UNORM                },
		{ TextureFormat::R16_SNORM,         VK_FORMAT_R16_SNORM                },
		{ TextureFormat::R16_FLOAT,         VK_FORMAT_R16_SFLOAT               },
		{ TextureFormat::BGRA4_UNORM,       VK_FORMAT_B4G4R4A4_UNORM_PACK16    },
		{ TextureFormat::B5G6R5_UNORM,      VK_FORMAT_B5G6R5_UNORM_PACK16      },
		{ TextureFormat::B5G5R5A1_UNORM,    VK_FORMAT_B5G5R5A1_UNORM_PACK16    },
		{ TextureFormat::RGBA8_UINT,        VK_FORMAT_R8G8B8A8_UINT            },
		{ TextureFormat::RGBA8_SINT,        VK_FORMAT_R8G8B8A8_SINT            },
		{ TextureFormat::RGBA8_UNORM,       VK_FORMAT_R8G8B8A8_UNORM           },
		{ TextureFormat::RGBA8_SNORM,       VK_FORMAT_R8G8B8A8_SNORM           },
		{ TextureFormat::BGRA8_UNORM,       VK_FORMAT_B8G8R8A8_UNORM           },
		{ TextureFormat::RGBA8_SRGB,        VK_FORMAT_R8G8B8A8_SRGB            },
		{ TextureFormat::BGRA8_SRGB,		VK_FORMAT_B8G8R8A8_SRGB            },
		{ TextureFormat::R10G10B10A2_UNORM, VK_FORMAT_A2B10G10R10_UNORM_PACK32 },
		{ TextureFormat::R11G11B10_FLOAT,   VK_FORMAT_B10G11R11_UFLOAT_PACK32  },
		{ TextureFormat::RG16_UINT,         VK_FORMAT_R16G16_UINT              },
		{ TextureFormat::RG16_SINT,         VK_FORMAT_R16G16_SINT              },
		{ TextureFormat::RG16_UNORM,        VK_FORMAT_R16G16_UNORM             },
		{ TextureFormat::RG16_SNORM,        VK_FORMAT_R16G16_SNORM             },
		{ TextureFormat::RG16_FLOAT,        VK_FORMAT_R16G16_SFLOAT            },
		{ TextureFormat::R32_UINT,          VK_FORMAT_R32_UINT                 },
		{ TextureFormat::R32_SINT,          VK_FORMAT_R32_SINT                 },
		{ TextureFormat::R32_FLOAT,         VK_FORMAT_R32_SFLOAT               },
		{ TextureFormat::RGBA16_UINT,       VK_FORMAT_R16G16B16A16_UINT        },
		{ TextureFormat::RGBA16_SINT,       VK_FORMAT_R16G16B16A16_SINT        },
		{ TextureFormat::RGBA16_FLOAT,      VK_FORMAT_R16G16B16A16_SFLOAT      },
		{ TextureFormat::RGBA16_UNORM,      VK_FORMAT_R16G16B16A16_UNORM       },
		{ TextureFormat::RGBA16_SNORM,      VK_FORMAT_R16G16B16A16_SNORM       },
		{ TextureFormat::RG32_UINT,         VK_FORMAT_R32G32_UINT              },
		{ TextureFormat::RG32_SINT,         VK_FORMAT_R32G32_SINT              },
		{ TextureFormat::RG32_FLOAT,        VK_FORMAT_R32G32_SFLOAT            },
		{ TextureFormat::RGB32_UINT,        VK_FORMAT_R32G32B32_UINT           },
		{ TextureFormat::RGB32_SINT,        VK_FORMAT_R32G32B32_SINT           },
		{ TextureFormat::RGB32_FLOAT,       VK_FORMAT_R32G32B32_SFLOAT         },
		{ TextureFormat::RGBA32_UINT,       VK_FORMAT_R32G32B32A32_UINT        },
		{ TextureFormat::RGBA32_SINT,       VK_FORMAT_R32G32B32A32_SINT        },
		{ TextureFormat::RGBA32_FLOAT,      VK_FORMAT_R32G32B32A32_SFLOAT      },
		{ TextureFormat::D16_UNORM,         VK_FORMAT_D16_UNORM                },
		{ TextureFormat::D24_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT        },
		{ TextureFormat::D32_UNORM,         VK_FORMAT_D32_SFLOAT               },
		{ TextureFormat::D32_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT       },
		{ TextureFormat::BC1_UNORM,         VK_FORMAT_BC1_RGBA_UNORM_BLOCK     },
		{ TextureFormat::BC1_UNORM_SRGB,    VK_FORMAT_BC1_RGBA_SRGB_BLOCK      },
		{ TextureFormat::BC2_UNORM,         VK_FORMAT_BC2_UNORM_BLOCK          },
		{ TextureFormat::BC2_UNORM_SRGB,    VK_FORMAT_BC2_SRGB_BLOCK           },
		{ TextureFormat::BC3_UNORM,         VK_FORMAT_BC3_UNORM_BLOCK          },
		{ TextureFormat::BC3_UNORM_SRGB,    VK_FORMAT_BC3_SRGB_BLOCK           },
		{ TextureFormat::BC4_UNORM,         VK_FORMAT_BC4_UNORM_BLOCK          },
		{ TextureFormat::BC4_SNORM,         VK_FORMAT_BC4_SNORM_BLOCK          },
		{ TextureFormat::BC5_UNORM,         VK_FORMAT_BC5_UNORM_BLOCK          },
		{ TextureFormat::BC5_SNORM,         VK_FORMAT_BC5_SNORM_BLOCK          },
		{ TextureFormat::BC6H_UFLOAT,       VK_FORMAT_BC6H_UFLOAT_BLOCK        },
		{ TextureFormat::BC6H_SFLOAT,       VK_FORMAT_BC6H_SFLOAT_BLOCK        },
		{ TextureFormat::BC7_UNORM,         VK_FORMAT_BC7_UNORM_BLOCK          },
		{ TextureFormat::BC7_UNORM_SRGB,    VK_FORMAT_BC7_SRGB_BLOCK           },

	} };

	static const std::unordered_map<VkFormat, TextureFormat> _VkFmtToTexFmtMap =
	{ 
		{ VK_FORMAT_UNDEFINED                ,TextureFormat::UNKNOWN          },
		{ VK_FORMAT_R8_UINT                  ,TextureFormat::R8_UINT          },
		{ VK_FORMAT_R8_SINT                  ,TextureFormat::R8_SINT          },
		{ VK_FORMAT_R8_UNORM                 ,TextureFormat::R8_UNORM         },
		{ VK_FORMAT_R8_SNORM                 ,TextureFormat::R8_SNORM         },
		{ VK_FORMAT_R8G8_UINT                ,TextureFormat::RG8_UINT         },
		{ VK_FORMAT_R8G8_SINT                ,TextureFormat::RG8_SINT         },
		{ VK_FORMAT_R8G8_UNORM               ,TextureFormat::RG8_UNORM        },
		{ VK_FORMAT_R8G8_SNORM               ,TextureFormat::RG8_SNORM        },
		{ VK_FORMAT_R16_UINT                 ,TextureFormat::R16_UINT         },
		{ VK_FORMAT_R16_SINT                 ,TextureFormat::R16_SINT         },
		{ VK_FORMAT_R16_UNORM                ,TextureFormat::R16_UNORM        },
		{ VK_FORMAT_R16_SNORM                ,TextureFormat::R16_SNORM        },
		{ VK_FORMAT_R16_SFLOAT               ,TextureFormat::R16_FLOAT        },
		{ VK_FORMAT_B4G4R4A4_UNORM_PACK16    ,TextureFormat::BGRA4_UNORM      },
		{ VK_FORMAT_B5G6R5_UNORM_PACK16      ,TextureFormat::B5G6R5_UNORM     },
		{ VK_FORMAT_B5G5R5A1_UNORM_PACK16    ,TextureFormat::B5G5R5A1_UNORM   },
		{ VK_FORMAT_R8G8B8A8_UINT            ,TextureFormat::RGBA8_UINT       },
		{ VK_FORMAT_R8G8B8A8_SINT            ,TextureFormat::RGBA8_SINT       },
		{ VK_FORMAT_R8G8B8A8_UNORM           ,TextureFormat::RGBA8_UNORM      },
		{ VK_FORMAT_R8G8B8A8_SNORM           ,TextureFormat::RGBA8_SNORM      },
		{ VK_FORMAT_B8G8R8A8_UNORM           ,TextureFormat::BGRA8_UNORM      },
		{ VK_FORMAT_R8G8B8A8_SRGB            ,TextureFormat::RGBA8_SRGB       },
		{ VK_FORMAT_B8G8R8A8_SRGB            ,TextureFormat::BGRA8_SRGB		  },
		{ VK_FORMAT_A2B10G10R10_UNORM_PACK32 ,TextureFormat::R10G10B10A2_UNORM},
		{ VK_FORMAT_B10G11R11_UFLOAT_PACK32  ,TextureFormat::R11G11B10_FLOAT  },
		{ VK_FORMAT_R16G16_UINT              ,TextureFormat::RG16_UINT        },
		{ VK_FORMAT_R16G16_SINT              ,TextureFormat::RG16_SINT        },
		{ VK_FORMAT_R16G16_UNORM             ,TextureFormat::RG16_UNORM       },
		{ VK_FORMAT_R16G16_SNORM             ,TextureFormat::RG16_SNORM       },
		{ VK_FORMAT_R16G16_SFLOAT            ,TextureFormat::RG16_FLOAT       },
		{ VK_FORMAT_R32_UINT                 ,TextureFormat::R32_UINT         },
		{ VK_FORMAT_R32_SINT                 ,TextureFormat::R32_SINT         },
		{ VK_FORMAT_R32_SFLOAT               ,TextureFormat::R32_FLOAT        },
		{ VK_FORMAT_R16G16B16A16_UINT        ,TextureFormat::RGBA16_UINT      },
		{ VK_FORMAT_R16G16B16A16_SINT        ,TextureFormat::RGBA16_SINT      },
		{ VK_FORMAT_R16G16B16A16_SFLOAT      ,TextureFormat::RGBA16_FLOAT     },
		{ VK_FORMAT_R16G16B16A16_UNORM       ,TextureFormat::RGBA16_UNORM     },
		{ VK_FORMAT_R16G16B16A16_SNORM       ,TextureFormat::RGBA16_SNORM     },
		{ VK_FORMAT_R32G32_UINT              ,TextureFormat::RG32_UINT        },
		{ VK_FORMAT_R32G32_SINT              ,TextureFormat::RG32_SINT        },
		{ VK_FORMAT_R32G32_SFLOAT            ,TextureFormat::RG32_FLOAT       },
		{ VK_FORMAT_R32G32B32_UINT           ,TextureFormat::RGB32_UINT       },
		{ VK_FORMAT_R32G32B32_SINT           ,TextureFormat::RGB32_SINT       },
		{ VK_FORMAT_R32G32B32_SFLOAT         ,TextureFormat::RGB32_FLOAT      },
		{ VK_FORMAT_R32G32B32A32_UINT        ,TextureFormat::RGBA32_UINT      },
		{ VK_FORMAT_R32G32B32A32_SINT        ,TextureFormat::RGBA32_SINT      },
		{ VK_FORMAT_R32G32B32A32_SFLOAT      ,TextureFormat::RGBA32_FLOAT     },
		{ VK_FORMAT_D16_UNORM                ,TextureFormat::D16_UNORM        },
		{ VK_FORMAT_D24_UNORM_S8_UINT        ,TextureFormat::D24_UNORM_S8_UINT},
		{ VK_FORMAT_D32_SFLOAT               ,TextureFormat::D32_UNORM        },
		{ VK_FORMAT_D32_SFLOAT_S8_UINT       ,TextureFormat::D32_UNORM_S8_UINT},
		{ VK_FORMAT_BC1_RGBA_UNORM_BLOCK     ,TextureFormat::BC1_UNORM        },
		{ VK_FORMAT_BC1_RGBA_SRGB_BLOCK      ,TextureFormat::BC1_UNORM_SRGB   },
		{ VK_FORMAT_BC2_UNORM_BLOCK          ,TextureFormat::BC2_UNORM        },
		{ VK_FORMAT_BC2_SRGB_BLOCK           ,TextureFormat::BC2_UNORM_SRGB   },
		{ VK_FORMAT_BC3_UNORM_BLOCK          ,TextureFormat::BC3_UNORM        },
		{ VK_FORMAT_BC3_SRGB_BLOCK           ,TextureFormat::BC3_UNORM_SRGB   },
		{ VK_FORMAT_BC4_UNORM_BLOCK          ,TextureFormat::BC4_UNORM        },
		{ VK_FORMAT_BC4_SNORM_BLOCK          ,TextureFormat::BC4_SNORM        },
		{ VK_FORMAT_BC5_UNORM_BLOCK          ,TextureFormat::BC5_UNORM        },
		{ VK_FORMAT_BC5_SNORM_BLOCK          ,TextureFormat::BC5_SNORM        },
		{ VK_FORMAT_BC6H_UFLOAT_BLOCK        ,TextureFormat::BC6H_UFLOAT      },
		{ VK_FORMAT_BC6H_SFLOAT_BLOCK        ,TextureFormat::BC6H_SFLOAT      },
		{ VK_FORMAT_BC7_UNORM_BLOCK          ,TextureFormat::BC7_UNORM        },
		{ VK_FORMAT_BC7_SRGB_BLOCK           ,TextureFormat::BC7_UNORM_SRGB   },

	};

	// TextureFormat mapping table. The rows must be in the exactly same order as TextureFormat enum members are defined.
	static const FormatInfo c_FormatInfo[] = {
	//        format                   name             bytes blk   red   green   blue  alpha  depth  stencl 
		{ TextureFormat::UNKNOWN,           "UNKNOWN",           0,   0,  false, false, false, false, false, false,  },
		{ TextureFormat::R8_UINT,           "R8_UINT",           1,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::R8_SINT,           "R8_SINT",           1,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::R8_UNORM,          "R8_UNORM",          1,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::R8_SNORM,          "R8_SNORM",          1,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::RG8_UINT,          "RG8_UINT",          2,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RG8_SINT,          "RG8_SINT",          2,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RG8_UNORM,         "RG8_UNORM",         2,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RG8_SNORM,         "RG8_SNORM",         2,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::R16_UINT,          "R16_UINT",          2,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::R16_SINT,          "R16_SINT",          2,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::R16_UNORM,         "R16_UNORM",         2,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::R16_SNORM,         "R16_SNORM",         2,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::R16_FLOAT,         "R16_FLOAT",         2,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::BGRA4_UNORM,       "BGRA4_UNORM",       2,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::B5G6R5_UNORM,      "B5G6R5_UNORM",      2,   1,  true,  true,  true,  false, false, false,  },
		{ TextureFormat::B5G5R5A1_UNORM,    "B5G5R5A1_UNORM",    2,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA8_UINT,        "RGBA8_UINT",        4,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA8_SINT,        "RGBA8_SINT",        4,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA8_UNORM,       "RGBA8_UNORM",       4,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA8_SNORM,       "RGBA8_SNORM",       4,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::BGRA8_UNORM,       "BGRA8_UNORM",       4,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA8_SRGB,      "RGBA8_SRGB",      4,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::BGRA8_SRGB,      "BGRA8_SRGB",      4,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::R10G10B10A2_UNORM, "R10G10B10A2_UNORM", 4,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::R11G11B10_FLOAT,   "R11G11B10_FLOAT",   4,   1,  true,  true,  true,  false, false, false,  },
		{ TextureFormat::RG16_UINT,         "RG16_UINT",         4,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RG16_SINT,         "RG16_SINT",         4,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RG16_UNORM,        "RG16_UNORM",        4,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RG16_SNORM,        "RG16_SNORM",        4,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RG16_FLOAT,        "RG16_FLOAT",        4,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::R32_UINT,          "R32_UINT",          4,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::R32_SINT,          "R32_SINT",          4,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::R32_FLOAT,         "R32_FLOAT",         4,   1,  true,  false, false, false, false, false,  },
		{ TextureFormat::RGBA16_UINT,       "RGBA16_UINT",       8,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA16_SINT,       "RGBA16_SINT",       8,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA16_FLOAT,      "RGBA16_FLOAT",      8,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA16_UNORM,      "RGBA16_UNORM",      8,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA16_SNORM,      "RGBA16_SNORM",      8,   1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RG32_UINT,         "RG32_UINT",         8,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RG32_SINT,         "RG32_SINT",         8,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RG32_FLOAT,        "RG32_FLOAT",        8,   1,  true,  true,  false, false, false, false,  },
		{ TextureFormat::RGB32_UINT,        "RGB32_UINT",        12,  1,  true,  true,  true,  false, false, false,  },
		{ TextureFormat::RGB32_SINT,        "RGB32_SINT",        12,  1,  true,  true,  true,  false, false, false,  },
		{ TextureFormat::RGB32_FLOAT,       "RGB32_FLOAT",       12,  1,  true,  true,  true,  false, false, false,  },
		{ TextureFormat::RGBA32_UINT,       "RGBA32_UINT",       16,  1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA32_SINT,       "RGBA32_SINT",       16,  1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::RGBA32_FLOAT,      "RGBA32_FLOAT",      16,  1,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::D16_UNORM,         "D16_UNORM",         2,   1,  false, false, false, false, true,  false,  },
		{ TextureFormat::D24_UNORM_S8_UINT, "D24_UNORM_S8_UINT", 4,   1,  false, false, false, false, true,  true,   },
		{ TextureFormat::D32_UNORM,         "D32_UNORM",         4,   1,  false, false, false, false, true,  false,  },
		{ TextureFormat::D32_UNORM_S8_UINT, "D32_UNORM_S8_UINT", 8,   1,  false, false, false, false, true,  true,   },
		{ TextureFormat::BC1_UNORM,         "BC1_UNORM",         8,   4,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::BC1_UNORM_SRGB,    "BC1_UNORM_SRGB",    8,   4,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::BC2_UNORM,         "BC2_UNORM",         16,  4,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::BC2_UNORM_SRGB,    "BC2_UNORM_SRGB",    16,  4,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::BC3_UNORM,         "BC3_UNORM",         16,  4,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::BC3_UNORM_SRGB,    "BC3_UNORM_SRGB",    16,  4,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::BC4_UNORM,         "BC4_UNORM",         8,   4,  true,  false, false, false, false, false,  },
		{ TextureFormat::BC4_SNORM,         "BC4_SNORM",         8,   4,  true,  false, false, false, false, false,  },
		{ TextureFormat::BC5_UNORM,         "BC5_UNORM",         16,  4,  true,  true,  false, false, false, false,  },
		{ TextureFormat::BC5_SNORM,         "BC5_SNORM",         16,  4,  true,  true,  false, false, false, false,  },
		{ TextureFormat::BC6H_UFLOAT,       "BC6H_UFLOAT",       16,  4,  true,  true,  true,  false, false, false,  },
		{ TextureFormat::BC6H_SFLOAT,       "BC6H_SFLOAT",       16,  4,  true,  true,  true,  false, false, false,  },
		{ TextureFormat::BC7_UNORM,         "BC7_UNORM",         16,  4,  true,  true,  true,  true,  false, false,  },
		{ TextureFormat::BC7_UNORM_SRGB,    "BC7_UNORM_SRGB",    16,  4,  true,  true,  true,  true,  false, false,  },
	};
	const FormatInfo& getFormatInfo(TextureFormat format)
	{
		static_assert(sizeof(c_FormatInfo) / sizeof(FormatInfo) == size_t(TextureFormat::COUNT),
			"The format info table doesn't have the right number of elements");
		const FormatInfo& info = c_FormatInfo[uint32_t(format)];
		assert(info.format == format);
		return info;
	}

	VkImageUsageFlags getVkImageUsageFlags(const TextureDesc& desc)
	{
		const FormatInfo& formatInfo = getFormatInfo(desc.format);

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

	VkImageCreateFlags getVkImageCreateFlags(TextureDimension dimension)
	{
		VkImageCreateFlags flags = 0;
		if (dimension == TextureDimension::TextureCube ||
			dimension == TextureDimension::TextureCubeArray)
		{
			flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		}
		return flags;
	}

	VkSampleCountFlagBits getVkImageSampleCount(const TextureDesc& desc)
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

	VkImageType getVkImageType(TextureDimension dimension)
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

		case TextureDimension::Undefined:
		default:
			assert(!"Invalid Enumeration Value");
		}
	}

	VkImageViewType getVkImageViewType(TextureDimension dimension)
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

		case TextureDimension::Undefined:
		default:
			assert(!"Invalid Enumeration Value");
			return VK_IMAGE_VIEW_TYPE_2D;
		}
	}

	VkImageAspectFlags getVkAspectMask(VkFormat format)
	{
		switch (format)
		{
		case VK_FORMAT_S8_UINT:
			return VK_IMAGE_ASPECT_STENCIL_BIT;
			break;
		case VK_FORMAT_D16_UNORM:
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

	VkFormat textureFormatToVkFormat(TextureFormat format)
	{
		assert(format < TextureFormat::COUNT);
		assert(_texFmtToVkFmtMap[uint32_t(format)].rhiFormat == format);

		return _texFmtToVkFmtMap[uint32_t(format)].vkFormat;
	}

	TextureFormat vkFormatToTextureFormat(VkFormat format)
	{
		auto res = _VkFmtToTexFmtMap.find(format);
		if (res != _VkFmtToTexFmtMap.end())
		{
			return res->second;
		}
		else
		{
			return TextureFormat::UNKNOWN;
		}
	}

	TextureVk::~TextureVk()
	{
		if (managed && image)
		{
			vmaDestroyImage(m_Allocator, image, allocation);
		}

		if (view)
		{
			vkDestroyImageView(m_Context.device, view, nullptr);
		}
	}
}