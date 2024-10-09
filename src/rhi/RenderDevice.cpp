#include "RenderDevice.h"

#include <sstream>
#include <iostream>
#include <cassert>

using namespace rhi;

namespace rhi
{
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		RenderDevice* rd = reinterpret_cast<RenderDevice*>(pUserData);

		MessageSeverity serverity = MessageSeverity::Info;

		if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
			serverity = MessageSeverity::Verbose;
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
			serverity = MessageSeverity::Info;
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			serverity = MessageSeverity::Warning;
		}
		else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
			serverity = MessageSeverity::Error;
		}

		std::stringstream debugMessage;
		if (pCallbackData->pMessageIdName) {
			debugMessage  << "[" << pCallbackData->messageIdNumber << "][" << pCallbackData->pMessageIdName << "] : " << pCallbackData->pMessage;
		}
		else {
			debugMessage  << "[" << pCallbackData->messageIdNumber << "] : " << pCallbackData->pMessage;
		}

		rd->GetMessageCallback()(serverity, debugMessage.str().c_str());

		// The return value of this callback controls whether the Vulkan call that caused the validation message will be aborted or not
		// We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message to abort
		// If you instead want to have calls abort, pass in VK_TRUE and the function will return VK_ERROR_VALIDATION_FAILED_EXT
		return VK_FALSE;
	}

	bool RenderDevice::CreateInstance(bool enableValidationLayer)
	{
		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };


		instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

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

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "rhi";
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo instanceCreateInfo{};
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pApplicationInfo = &appInfo;

		if (enableValidationLayer) {
			VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI{};
			debugUtilsMessengerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugUtilsMessengerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugUtilsMessengerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
			debugUtilsMessengerCI.pfnUserCallback = DebugMessageCallback;
			debugUtilsMessengerCI.pUserData = this;
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
				m_Context.Warning("Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
			}
		}

		instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

		VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Context.instace);
		if (result != VK_SUCCESS)
		{
			m_Context.Error("Failed to create a Vulkan instance");
			return false;
		}
		return true;
	}


	bool rhi::RenderDevice::PickPhysicalDevice()
	{
		assert(m_Context.instace != VK_NULL_HANDLE);

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Context.instace, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			m_Context.Error("No device with Vulkan support found");
			return false;
		}

		std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
		vkEnumeratePhysicalDevices(m_Context.instace, &deviceCount, physicalDevices.data());

		// pick the first discrete GPU if it exists, otherwise the first integrated GPU
		for (const VkPhysicalDevice& physicalDevice : physicalDevices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				m_Context.physicalDevice = physicalDevice;
				return true;
			}
		}

		m_Context.physicalDevice = physicalDevices[0];
		return true;
	}

	bool rhi::RenderDevice::CreateDevice()
	{
		assert(m_Context.physicalDevice != VK_NULL_HANDLE);
		// find queue family
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_Context.physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyPropertieses(queueFamilyCount);

		for (uint32_t i = 0; i < queueFamilyPropertieses.size(); i++)
		{
			const auto& queueFamilyProps = queueFamilyPropertieses[i];

			if (!m_QueueFamilyIndices.graphics.has_value())
			{
				if (queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					m_QueueFamilyIndices.graphics = i;
				}
			}

			if (!m_QueueFamilyIndices.compute.has_value())
			{
				if (queueFamilyProps.queueFlags & VK_QUEUE_COMPUTE_BIT &&
					queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					m_QueueFamilyIndices.compute = i;
				}
			}

			if (!m_QueueFamilyIndices.transfer.has_value())
			{
				if (queueFamilyProps.queueFlags & VK_QUEUE_TRANSFER_BIT &&
					queueFamilyProps.queueFlags & VK_QUEUE_COMPUTE_BIT &&
					queueFamilyProps.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					m_QueueFamilyIndices.transfer = i;
				}
			}
		}

		std::vector<uint32_t> queueFamilies{ m_QueueFamilyIndices.graphics.value(),
			m_QueueFamilyIndices.compute.value(),
			m_QueueFamilyIndices.transfer.value() };

		float priority = 1.f;
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfo(queueFamilies.size());
		for (uint32_t queueFamilyIndex : queueFamilies)
		{
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = queueFamilyIndex;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &priority;
			queueCreateInfo.push_back(queueInfo);
		}

		// Create the logical device
		std::vector<const char*> deviceExtensions;

		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.textureCompressionBC = true;
		// todo: add more feature 

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfo.size());
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfo.data();

		VkResult result = vkCreateDevice(m_Context.physicalDevice, &deviceCreateInfo, nullptr, &m_Context.device);
		if (result != VK_SUCCESS)
		{
			m_Context.Error("Failed to create a Vulkan device");
			return false;
		}

		vkGetDeviceQueue(m_Context.device, m_QueueFamilyIndices.graphics.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Context.device, m_QueueFamilyIndices.compute.value(), 0, &m_ComputeQueue);
		vkGetDeviceQueue(m_Context.device, m_QueueFamilyIndices.transfer.value(), 0, &m_TransferQueue);
		return true;
	}

	void rhi::RenderDevice::DestroyDebugUtilsMessenger()
	{
		assert(m_Context.instace != VK_NULL_HANDLE && m_DebugUtilsMessenger != VK_NULL_HANDLE);

		auto vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_Context.instace, "vkDestroyDebugUtilsMessengerEXT"));

		vkDestroyDebugUtilsMessengerEXT(m_Context.instace, m_DebugUtilsMessenger, nullptr);
	}

	RenderDevice* RenderDevice::Create(const RenderDeviceCreateInfo& createInfo)
	{
		auto renderDevice = new RenderDevice();
		renderDevice->m_Context.messageCallBack = createInfo.messageCallBack;

		if (!renderDevice->CreateInstance(createInfo.enableValidationLayer))
		{
			return nullptr;
		}

		if (createInfo.enableValidationLayer)
		{
			auto vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(renderDevice->m_Context.instace, "vkCreateDebugUtilsMessengerEXT"));

			VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI{};
			debugUtilsMessengerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugUtilsMessengerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugUtilsMessengerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
			debugUtilsMessengerCI.pfnUserCallback = DebugMessageCallback;
			debugUtilsMessengerCI.pUserData = renderDevice;
			VkResult result = vkCreateDebugUtilsMessengerEXT(renderDevice->m_Context.instace, &debugUtilsMessengerCI, nullptr, &renderDevice->m_DebugUtilsMessenger);
			assert(result == VK_SUCCESS);
		}

		if (!renderDevice->PickPhysicalDevice())
		{
			return nullptr;
		}

		if (!renderDevice->CreateDevice())
		{
			return nullptr;
		}
		return nullptr;
	}


	Texture* RenderDevice::CreateTexture(const TextureDesc& desc, VkImage imageHandle)
	{
		Texture* tex = new Texture(m_Context, m_Allocator);
		tex->image = imageHandle;
		tex->managed = false;
		tex->format = GetVkFormat(desc.format);

		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = GetVkImageType(desc.dimension);
		imageCreateInfo.extent = { desc.width, desc.height, desc.depth };
		imageCreateInfo.mipLevels = desc.mipLevels;
		imageCreateInfo.arrayLayers = desc.depth;
		imageCreateInfo.format = tex->format;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = GetVkImageUsageFlags(desc);
	}
}

