#pragma once

#include "rhi/SwapChain.h"

namespace rhi
{
	class SwapChainVk : public ISwapChain
	{
	public:
		void SetRenderDevice(RenderDevice& renderDevice);
		void InitSurface(void* platformHandle, void* platformWindow);
		void Create(uint32_t* width, uint32_t* height, Format swapChainFormat, bool vsync);
	};
}