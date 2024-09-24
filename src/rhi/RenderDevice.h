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

	using MessageCallBack = std::function<void(MessageSeverity, const char*)>;

	struct RenderDeviceCreateInfo
	{
		MessageCallBack messageCallBack;
		bool enableValidationLayer;
		std::vector<const char*> requiredExtensions;
	};

	class RenderDevice
	{
	public:
		explicit RenderDevice(const RenderDeviceCreateInfo& createInfo);
		RenderDevice() = delete;
		~RenderDevice();
	private:
		VkInstance m_VulkanInstace;
		VkPhysicalDevice m_PhysicalDevice;
		MessageCallBack m_MessageCallBack;
	};

	std::unique_ptr<RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo);
}