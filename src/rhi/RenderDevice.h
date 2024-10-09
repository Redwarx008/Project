#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <optional>

#include <vulkan/vulkan.h>

#include "RHI.h"
#include "VkContext.h"
#include "SwapChain.h"
#include "Texture.h"

namespace rhi
{

	struct RenderDeviceCreateInfo
	{
		MessageCallback messageCallBack;
		bool enableValidationLayer;
		// enable instance features
	};


	class RenderDevice
	{
	public:
		friend class SwapChain;
	public:
		static RenderDevice* Create(const RenderDeviceCreateInfo& createInfo);

		MessageCallback GetMessageCallback() { return m_Context.messageCallBack; }
		~RenderDevice() = default;
	private:
		RenderDevice();
		bool CreateInstance(bool enableValidationLayer);
		bool PickPhysicalDevice();
		bool CreateDevice();
		void DestroyDebugUtilsMessenger();

		VkContext m_Context;

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
	public:
		Texture* CreateTexture(const TextureDesc& desc, VkImage imageHandle);
	};
}