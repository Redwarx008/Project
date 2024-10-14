#pragma once

#include "rhi/SwapChain.h"
#include "TextureVk.h"
#include <vulkan/vulkan.h>

#include <vector>
#include <memory>

namespace rhi
{
	class RenderDeviceVk;

	class SwapChainVk final : public ISwapChain
	{
	public:
		static SwapChainVk* create(const SwapChainCreateInfo& swapChainCI);
	private:
		SwapChainVk() = default;
		bool createSurface(void* platformHandle, void* platformWindow);
		bool createVkSwapChain();

		uint32_t m_Width;
		uint32_t m_Height;

		Format m_ColorFormat;

		bool m_VSyncEnabled;

		RenderDeviceVk& m_RenderDevice;
		VkSurfaceKHR m_WindowSurface;
		VkSwapchainKHR m_SwapChain;

		std::vector<std::unique_ptr<ITexture>> m_ColorTextures;
	};
}