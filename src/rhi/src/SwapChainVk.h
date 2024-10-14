#pragma once

#include "rhi/SwapChain.h"
#include "TextureVk.h"
#include <vulkan/vulkan.h>

#include <vector>

namespace rhi
{
	class RenderDeviceVk;

	class SwapChainVk final : public ISwapChain
	{
	public:
		static SwapChainVk* create(const SwapChainCreateInfo& swapChainCI);
	private:
		SwapChainVk() = default;
		bool createSurface();
		bool createVkSwapChain();

		RenderDeviceVk& m_RenderDevice;
		VkSurfaceKHR m_WindowSurface;
		VkSwapchainKHR m_SwapChain;

		std::vector<TextureVk> m_SwapChainTextures;
	};
}