#pragma once

#include "rhi/RenderDevice.h"
#if defined RHI_ENABLE_THREAD_RECORDING
#include <mutex>
#endif
#include <vk_mem_alloc.h>
#include "ContextVk.h"
#include "CommandListVk.h"

namespace rhi
{
	class TextureVk;

	class RenderDeviceVk final : public IRenderDevice
	{
	public:
		~RenderDeviceVk();
		// Internal methods
		static RenderDeviceVk* create(const RenderDeviceDesc& desc);

		const ContextVk& getVkContext() const { return m_Context; }
		uint32_t getQueueFamilyIndex() const { return m_QueueFamilyIndex; }
		CommandBuffer* getOrCreateCommandBuffer();
		void setSwapChainImageAvailableSeamaphore(const VkSemaphore& semaphore);
		TextureVk* createTextureWithExistImage(const TextureDesc& desc, VkImage image);
		// Interface implementation
		void waitIdle() override;
		ITexture* createTexture(const TextureDesc& desc) override;
		IBuffer* createBuffer(const BufferDesc& desc) override;
		IBuffer* createBuffer(const BufferDesc& desc, const void* data, size_t dataSize) override;
		ICommandList* createCommandList() override;

	private:
		RenderDeviceVk() = default;
		bool createInstance(bool enableValidationLayer);
		bool pickPhysicalDevice();
		bool createDevice();
		void destroyDebugUtilsMessenger();
#if defined RHI_ENABLE_THREAD_RECORDING
		std::mutex m_Mutex;
#endif
		ContextVk m_Context;
		VmaAllocator m_Allocator;

		VkDebugUtilsMessengerEXT m_DebugUtilsMessenger;

		uint32_t m_QueueFamilyIndex;
		VkQueue m_Queue;

		VkSemaphore m_SwapChainImgAavailableSemaphore;

		std::vector<CommandBuffer*> m_CommandBufferInFlight;
		std::vector<CommandBuffer*> m_CommandBufferPool;
	};
}

