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
		TextureFormat preferredColorFormat = TextureFormat::RGBA8_SRGB;
		// use TextureFormat::UNKOWN to create swapChain without depth buffer.
		TextureFormat preferredDepthStencilFormat = TextureFormat::D32_UNORM_S8_UINT;
		uint32_t initialWidth = 0;
		uint32_t initialHeight = 0;
		bool enableVSync = true;
	};

	class ISwapChain
	{
	public:
		virtual ~ISwapChain() = default;
		virtual void BeginFrame() = 0;
		virtual void present() = 0;
		virtual void resize() = 0;
		virtual ITexture* getCurrentRenderTarget() = 0;
		virtual ITexture* getDepthStencil() = 0;
	};
}