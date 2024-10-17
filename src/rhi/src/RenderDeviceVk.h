#pragma once

#include "rhi/RenderDevice.h"
#if defined RHI_ENABLE_THREAD_RECORDING
#include <mutex>
#endif
#include <optional>
#include <vk_mem_alloc.h>
#include "ContextVk.h"
#include "TextureVk.h"
#include "CommandListVk.h"

namespace rhi
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphics;
		std::optional<uint32_t> compute;
		std::optional<uint32_t> transfer;
	};

	class RenderDeviceVk final : public IRenderDevice
	{
	public:
		~RenderDeviceVk();
		// Internal methods
		static RenderDeviceVk* create(const RenderDeviceDesc& desc);

		const ContextVk& getVkContext() const { return m_Context; }
		const QueueFamilyIndices& getQueueFamilyIndices() const { return m_QueueFamilyIndices; }
		CommandBuffer* getOrCreateCommandBuffer();
		void setSwapChainImageAvailableSeamaphore(const VkSemaphore& semaphore);
		TextureVk* createTextureWithExistImage(const TextureDesc& desc, VkImage image);
		// Interface implementation
		void waitIdle() override;
		ITexture* createTexture(const TextureDesc& desc) override;


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

		QueueFamilyIndices m_QueueFamilyIndices;

		VkQueue m_GraphicsQueue;
		VkQueue m_ComputeQueue;
		VkQueue m_TransferQueue;

		VkSemaphore m_SwapChainImgAavailableSemaphore;

		std::vector<CommandBuffer*> m_CommandBufferInFlight;
		std::vector<CommandBuffer*> m_CommandBufferPool;
	};
}

