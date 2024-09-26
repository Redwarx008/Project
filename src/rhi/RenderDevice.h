#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <vulkan/vulkan.h>

namespace rhi
{
	enum class MessageSeverity : uint8_t
	{
		Verbose,
		Info,
		Warning,
		Error,
		Fatal
	};

	using MessageCallback = std::function<void(MessageSeverity, const char*)>;

	struct RenderDeviceCreateInfo
	{
		MessageCallback messageCallBack = DefualtMessageCallback;
		bool enableValidationLayer;
		static void DefualtMessageCallback(MessageSeverity severity, const char* message)
		{
			// Select prefix depending on flags passed to the callback
			std::string prefix;

			if (severity == MessageSeverity::Verbose) {
#if defined(_WIN32)
				prefix = "\033[32m" + prefix + "\033[0m";
#endif
				prefix = "VERBOSE: ";
			}
			else if (severity == MessageSeverity::Info) {
				prefix = "INFO: ";
#if defined(_WIN32)
				prefix = "\033[36m" + prefix + "\033[0m";
#endif
			}
			else if (severity == MessageSeverity::Warning) {
				prefix = "WARNING: ";
#if defined(_WIN32)
				prefix = "\033[33m" + prefix + "\033[0m";
#endif
			}
			else if (severity == MessageSeverity::Error) {
				prefix = "ERROR: ";
#if defined(_WIN32)
				prefix = "\033[31m" + prefix + "\033[0m";
#endif
			}

			std::cout << prefix << message << "\n\n";

			fflush(stdout);
		}
	};

	class RenderDevice
	{
	public:
		friend std::unique_ptr<RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo);
	public:
		static RenderDevice* Create(const RenderDeviceCreateInfo& createInfo);
		void Message(MessageSeverity severity, const char* message) { m_MessageCallBack(severity, message); };
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