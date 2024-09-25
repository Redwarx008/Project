#include "RenderDevice.h"

#include <sstream>
#include <iostream>

using namespace rhi;

namespace rhi
{
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		//const RenderDevice* rd = reinterpret_cast<RenderDevice*>(pUserData);
		
		// Select prefix depending on flags passed to the callback
		std::string prefix;

		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
#if defined(_WIN32)
			prefix = "\033[32m" + prefix + "\033[0m";
#endif
			prefix = "VERBOSE: ";
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
			prefix = "INFO: ";
#if defined(_WIN32)
			prefix = "\033[36m" + prefix + "\033[0m";
#endif
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			prefix = "WARNING: ";
#if defined(_WIN32)
			prefix = "\033[33m" + prefix + "\033[0m";
#endif
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			prefix = "ERROR: ";
#if defined(_WIN32)
			prefix = "\033[31m" + prefix + "\033[0m";
#endif
		}


		// Display message to default output (console/logcat)
		std::stringstream debugMessage;
		if (pCallbackData->pMessageIdName) {
			debugMessage << prefix << "[" << pCallbackData->messageIdNumber << "][" << pCallbackData->pMessageIdName << "] : " << pCallbackData->pMessage;
		}
		else {
			debugMessage << prefix << "[" << pCallbackData->messageIdNumber << "] : " << pCallbackData->pMessage;
		}

#if defined(__ANDROID__)
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			LOGE("%s", debugMessage.str().c_str());
		}
		else {
			LOGD("%s", debugMessage.str().c_str());
		}
#else
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			std::cerr << debugMessage.str() << "\n\n";
		}
		else {
			std::cout << debugMessage.str() << "\n\n";
		}
		fflush(stdout);
#endif


		// The return value of this callback controls whether the Vulkan call that caused the validation message will be aborted or not
		// We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message to abort
		// If you instead want to have calls abort, pass in VK_TRUE and the function will return VK_ERROR_VALIDATION_FAILED_EXT
		return VK_FALSE;
	}
}

VkResult RenderDevice::CreateVulkanInstance(const RenderDeviceCreateInfo& createInfo)
{
	std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

	// Enable surface extensions depending on os
#if defined(_WIN32)
	instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
	instanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_DIRECT2DISPLAY)
	instanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
	instanceExtensions.push_back(VK_EXT_DIRECTFB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
	instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	instanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_HEADLESS_EXT)
	instanceExtensions.push_back(VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
#endif

	std::vector<const char*> supportedInstanceExtensions;
	// Get extensions supported by the instance and store for later use
	uint32_t extCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
	if (extCount > 0)
	{
		std::vector<VkExtensionProperties> extensions(extCount);
		if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
		{
			for (VkExtensionProperties& extension : extensions)
			{
				supportedInstanceExtensions.push_back(extension.extensionName);
			}
		}
	}

	if (createInfo.requiredInstanceExtensions.size() > 0)
	{
		for (const char* requiredExtension : createInfo.requiredInstanceExtensions)
		{
			// Output message if requested extension is not available
			if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), requiredExtension) == supportedInstanceExtensions.end())
			{
				std::stringstream ss;
				ss << "Enabled instance extension \"" << requiredExtension << "\" is not present at instance level\n";
				createInfo.messageCallBack(MessageSeverity::Warning, ss.str().c_str());
			}
			instanceExtensions.push_back(requiredExtension);
		}
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "rhi";
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;

	if (createInfo.enableValidationLayer) {
		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI{};
		debugUtilsMessengerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugUtilsMessengerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugUtilsMessengerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debugUtilsMessengerCI.pfnUserCallback = DebugMessageCallback;
		instanceCreateInfo.pNext = &debugUtilsMessengerCI;

		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		// The VK_LAYER_KHRONOS_validation contains all current validation functionality.
		// Note that on Android this layer requires at least NDK r20
		const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
		// Check if this layer is available at instance level
		uint32_t instanceLayerCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
		bool validationLayerPresent = false;
		for (VkLayerProperties& layer : instanceLayerProperties) {
			if (strcmp(layer.layerName, validationLayerName) == 0) {
				validationLayerPresent = true;
				break;
			}
		}
		if (validationLayerPresent) {
			instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
			instanceCreateInfo.enabledLayerCount = 1;
		}
		else {
			std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled";
		}
	}

	instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_VulkanInstace);
	return result;
}


std::unique_ptr<RenderDevice> rhi::CreateRenderDevice(const RenderDeviceCreateInfo& createInfo)
{
	RenderDevice* renderDevice = new RenderDevice();
	VkResult res = renderDevice->CreateVulkanInstance(createInfo);

	return renderDevice;
}