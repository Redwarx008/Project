#pragma once

#include "Texture.h"

namespace rhi
{
	class IRenderDevice;

	struct SwapChainCreateInfo
	{
		IRenderDevice& renderDevice;
		void* platformHandle;
		void* platformWindow;
		Format colorBufferFormat = Format::RGBA8_UNORM;
		Format depthStencilFormat = Format::X32G8_UINT;
		uint32_t initialWidth = 0;
		uint32_t initialHeight = 0;
	};

	class ISwapChain
	{
	public:
		virtual ~ISwapChain() = default;
		virtual void present() = 0;
		virtual void resize() = 0;
		virtual ITexture* getCurrentRenderTarget() = 0;
		virtual ITexture* getDepthStencil() = 0;
	};
}