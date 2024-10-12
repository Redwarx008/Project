#pragma once

#include "rhi/RenderDevice.h"

#include <optional>
#include <vk_mem_alloc.h>
#include "CommonVk.h"


namespace rhi
{
	class RenderDeviceVk final : public IRenderDevice
	{
	public:
		// Internal methods
		static RenderDeviceVk* Create(const RenderDeviceDesc& desc);
		MessageCallback GetMessageCallback() { return m_Context.messageCallBack; }
		ITexture* CreateTexture(const TextureDesc& desc, VkImage image);
		// Interface implementation

		ITexture* CreateTexture(const TextureDesc& desc) override;

	private:
		RenderDeviceVk() = default;
		bool CreateInstance(bool enableValidationLayer);
		bool PickPhysicalDevice();
		bool CreateDevice();
		void DestroyDebugUtilsMessenger();

		VkContext m_Context;
		VmaAllocator m_Allocator;

		VkDebugUtilsMessengerEXT m_DebugUtilsMessenger;

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphics;
			std::optional<uint32_t> compute;
			std::optional<uint32_t> transfer;
		};
		QueueFamilyIndices m_QueueFamilyIndices;

		VkQueue m_GraphicsQueue;
		VkQueue m_ComputeQueue;
		VkQueue m_TransferQueue;
	};
}

