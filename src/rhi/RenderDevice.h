#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <optional>

#include <vulkan/vulkan.h>
#include "SwapChain.h"
#include "Texture.h"

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
		MessageCallback messageCallBack;
		bool enableValidationLayer;
//		static void DefualtMessageCallback(MessageSeverity severity, const char* message)
//		{
//			// Select prefix depending on flags passed to the callback
//			std::string prefix;
//
//			if (severity == MessageSeverity::Verbose) {
//#if defined(_WIN32)
//				prefix = "\033[32m" + prefix + "\033[0m";
//#endif
//				prefix = "VERBOSE: ";
//			}
//			else if (severity == MessageSeverity::Info) {
//				prefix = "INFO: ";
//#if defined(_WIN32)
//				prefix = "\033[36m" + prefix + "\033[0m";
//#endif
//			}
//			else if (severity == MessageSeverity::Warning) {
//				prefix = "WARNING: ";
//#if defined(_WIN32)
//				prefix = "\033[33m" + prefix + "\033[0m";
//#endif
//			}
//			else if (severity == MessageSeverity::Error) {
//				prefix = "ERROR: ";
//#if defined(_WIN32)
//				prefix = "\033[31m" + prefix + "\033[0m";
//#endif
//			}
//
//			std::cout << prefix << message << "\n\n";
//
//			fflush(stdout);
//		}
	};


	class RenderDevice
	{
	public:
		friend std::unique_ptr<RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo);
		friend class SwapChain;
	public:
		static RenderDevice* Create(const RenderDeviceCreateInfo& createInfo);
		void Error(const std::string& message) { m_MessageCallBack(MessageSeverity::Error, message.c_str()); };
		void Warning(const std::string& message) { m_MessageCallBack(MessageSeverity::Warning, message.c_str()); }
		MessageCallback GetMessageCallback() { return m_MessageCallBack; }
		~RenderDevice() = default;
	private:
		RenderDevice();
		bool CreateInstance(bool enableValidationLayer);
		bool PickPhysicalDevice();
		bool CreateDevice();
		void DestroyDebugUtilsMessenger();

		VkInstance m_VKInstace;
		VkPhysicalDevice m_VKPhysicalDevice;
		VkDevice m_Device;

		VkDebugUtilsMessengerEXT m_DebugUtilsMessenger;
		MessageCallback m_MessageCallBack;

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
		Texture* CreateTexture2D(const TextureDesc& desc, VkImage imageHandle);
	};
}