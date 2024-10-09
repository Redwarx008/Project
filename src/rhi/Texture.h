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

	VkFormat convertFormat(Format format);

	VkImageAspectFlags getAspectMask(VkFormat format);

	struct TextureDesc
	{
		TextureDimension dimension = TextureDimension::Unknown;
		uint32_t width = 1;
		uint32_t height = 1;
		union 
		{
			/// For a 1D array or 2D array, number of array slices
			uint32_t arraySize = 1;
			/// For a 3D texture, number of depth slices
			uint32_t depth;
		};
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
		~Texture();
		TextureDesc desc;
		VkImage image;
		VkImageView	view;
		VkFormat format;

	private:
		const VkContext& m_Context;
		const VmaAllocator& m_Allocator;
	};
}