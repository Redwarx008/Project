#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "Texture.h"

namespace rhi
{
	class RenderDevice;

	class SwapChain
	{
	public:
		void SetRenderDevice(RenderDevice& renderDevice);
		void InitSurface(void* platformHandle, void* platformWindow);
		void Create(uint32_t* width, uint32_t* height, Format swapChainFormat, bool vsync);
		void Cleanup();
	private:
		RenderDevice* m_RenderDevice;
		VkSurfaceKHR m_WindowSurface;
		VkSwapchainKHR m_SwapChain;

		std::vector<Texture*> m_SwapChainTextures;
	};
}

