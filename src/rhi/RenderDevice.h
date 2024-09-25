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
		std::vector<const char*> requiredInstanceExtensions;
	};

	class RenderDevice
	{
	public:
		friend std::unique_ptr<RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo);
	public:
		~RenderDevice();
	private:
		RenderDevice();
		VkResult CreateVulkanInstance(const RenderDeviceCreateInfo& createInfo);

		VkInstance m_VulkanInstace;
		VkPhysicalDevice m_PhysicalDevice;
		MessageCallback m_MessageCallBack;

	};
}