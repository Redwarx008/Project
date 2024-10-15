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
		~SwapChainVk() {}
		static SwapChainVk* create(const SwapChainCreateInfo& swapChainCI);

		void present() override;
		void resize() override;
		ITexture* getCurrentRenderTarget() override;
		ITexture* getDepthStencil() override;
	private:
		SwapChainVk(RenderDeviceVk& renderDevice);
		void createSurface(void* platformHandle, void* platformWindow);
		void createVkSwapChain();

		uint32_t m_Width;
		uint32_t m_Height;

		TextureFormat m_ColorFormat;

		bool m_VSyncEnabled;

		RenderDeviceVk& m_RenderDevice;
		VkSurfaceKHR m_WindowSurface;
		VkSwapchainKHR m_SwapChain;

		uint32_t m_CurrentFrameInFlight = 0;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;

		std::vector<std::unique_ptr<ITexture>> m_ColorTextures;
	};
}