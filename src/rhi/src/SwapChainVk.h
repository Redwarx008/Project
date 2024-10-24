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
		~SwapChainVk();
		static SwapChainVk* create(const SwapChainCreateInfo& swapChainCI);
		void BeginFrame() override;
		void present() override;
		void resize() override;
		ITexture* getCurrentRenderTarget() override;
		ITexture* getDepthStencil() override;
	private:
		SwapChainVk(RenderDeviceVk& renderDevice);
		void createSurface(void* platformHandle, void* platformWindow);
		void createVkSwapChain();
		void recreateSwapChain();
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		Format m_ColorFormat {Format::UNKNOWN};
		Format m_DepthStencilFormat{ Format::UNKNOWN };

		bool m_VSyncEnabled = false;

		RenderDeviceVk& m_RenderDevice;
		VkSurfaceKHR m_WindowSurface = VK_NULL_HANDLE;
		VkSwapchainKHR m_SwapChain = VK_NULL_HANDLE;

		uint32_t m_CurrentFrameInFlight = 0;
		uint32_t m_SwapChainImageIndex = UINT32_MAX;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;

		std::vector<std::unique_ptr<ITexture>> m_ColorBuffers;
		std::unique_ptr<ITexture> m_DepthStencilBuffer;
	};
}