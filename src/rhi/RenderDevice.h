#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <vulkan/vulkan.h>

namespace rhi
{
	enum class MessageSeverity : uint8_t
	{
		Info,
		Warning,
		Error,
		Fatal
	};

	using MessageCallback = std::function<void(MessageSeverity, const char*)>;

	struct RenderDeviceCreateInfo
	{
		MessageCallback messageCallBack;
		bool enableValidationLayer;
	};

	class RenderDevice
	{
	public:
		friend std::unique_ptr<RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo);
	public:
		static RenderDevice* Create(const RenderDeviceCreateInfo& createInfo);
		~RenderDevice();
	private:
		RenderDevice();
		VkResult CreateInstance(bool enableValidationLayer);
		void PickPhysicalDevice();
		void CreateQueue();
		void DestroyDebugUtilsMessenger();
		VkInstance m_VKInstace;
		VkPhysicalDevice m_VKPhysicalDevice;
		VkDebugUtilsMessengerEXT m_DebugUtilsMessenger;
		MessageCallback m_MessageCallBack;

	};
}