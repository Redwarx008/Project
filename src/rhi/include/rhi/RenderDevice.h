#pragma once

#include <functional>

namespace rhi
{

	class ITexture;
	struct TextureDesc;

	class IRenderDevice
	{
	public:
		virtual ~IRenderDevice() = default;

		virtual ITexture* createTexture(const TextureDesc& desc) = 0;
	};
}