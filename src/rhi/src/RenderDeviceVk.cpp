#include "RenderDeviceVk.h"

#include <sstream>
#include "TextureVk.h"
#include "BufferVk.h"
#include "ErrorVk.h"

namespace rhi
{
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		//RenderDeviceVk* rd = reinterpret_cast<RenderDeviceVk*>(pUserData);

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
			debugMessage << "[" << pCallbackData->messageIdNumber << "][" << pCallbackData->pMessageIdName << "] : " << pCallbackData->pMessage;
		}
		else {
			debugMessage << "[" << pCallbackData->messageIdNumber << "] : " << pCallbackData->pMessage;
		}

		if (g_DebugMessageCallback)
		{
			g_DebugMessageCallback(serverity, debugMessage.str().c_str());
		}
		else
		{
			std::cerr << debugMessage.str().c_str();
		}
		// The return value of this callback controls whether the Vulkan call that caused the validation message will be aborted or not
		// We return VK_FALSE as we DON'T want Vulkan calls that cause a validation message to abort
		// If you instead want to have calls abort, pass in VK_TRUE and the function will return VK_ERROR_VALIDATION_FAILED_EXT
		return VK_FALSE;
	}

	bool RenderDeviceVk::createInstance(bool enableValidationLayer)
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
				LOG_ERROR("Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled");
			}
		}

		instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

		VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Context.instace);
		if (result != VK_SUCCESS)
		{
			LOG_ERROR("Failed to create a Vulkan instance");
			return false;
		}
		return true;
	}


	bool RenderDeviceVk::pickPhysicalDevice()
	{
		assert(m_Context.instace != VK_NULL_HANDLE);

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Context.instace, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			LOG_ERROR("No device with Vulkan support found");
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

	bool RenderDeviceVk::createDevice()
	{
		assert(m_Context.physicalDevice != VK_NULL_HANDLE);
		// find queue family
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_Context.physicalDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueFamilyPropertieses(queueFamilyCount);

		for (uint32_t i = 0; i < queueFamilyPropertieses.size(); i++)
		{
			const auto& queueFamilyProps = queueFamilyPropertieses[i];

			if (queueFamilyProps.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT))
			{
				m_QueueFamilyIndex = i;
				break;
			}
		}

		float priority = 1.f;
		VkDeviceQueueCreateInfo queueCI{};
		queueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCI.queueFamilyIndex = m_QueueFamilyIndex;;
		queueCI.queueCount = 1;
		queueCI.pQueuePriorities = &priority;

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
		deviceCreateInfo.queueCreateInfoCount = 1;
		deviceCreateInfo.pQueueCreateInfos = &queueCI;

		VkResult result = vkCreateDevice(m_Context.physicalDevice, &deviceCreateInfo, nullptr, &m_Context.device);
		if (result != VK_SUCCESS)
		{
			LOG_ERROR("Failed to create a Vulkan device");
			return false;
		}

		vkGetDeviceQueue(m_Context.device, m_QueueFamilyIndex, 0, &m_Queue);
		return true;
	}

	void RenderDeviceVk::destroyDebugUtilsMessenger()
	{
		assert(m_Context.instace && m_DebugUtilsMessenger);

		auto vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_Context.instace, "vkDestroyDebugUtilsMessengerEXT"));

		vkDestroyDebugUtilsMessengerEXT(m_Context.instace, m_DebugUtilsMessenger, nullptr);
	}

	RenderDeviceVk* RenderDeviceVk::create(const RenderDeviceDesc& desc)
	{
		auto renderDevice = new RenderDeviceVk();
		g_DebugMessageCallback = desc.messageCallback;

		if (!renderDevice->createInstance(desc.enableValidationLayer))
		{
			return nullptr;
		}

		if (desc.enableValidationLayer)
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

		if (!renderDevice->pickPhysicalDevice())
		{
			return nullptr;
		}

		if (!renderDevice->createDevice())
		{
			return nullptr;
		}

		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
		allocatorCreateInfo.physicalDevice = renderDevice->m_Context.physicalDevice;
		allocatorCreateInfo.device = renderDevice->m_Context.device;
		allocatorCreateInfo.instance = renderDevice->m_Context.instace;
		vmaCreateAllocator(&allocatorCreateInfo, &renderDevice->m_Allocator);
		return nullptr;
	}

	RenderDeviceVk::~RenderDeviceVk()
	{
		destroyDebugUtilsMessenger();
		vmaDestroyAllocator(m_Allocator);
	}

	void RenderDeviceVk::waitIdle()
	{
		vkDeviceWaitIdle(m_Context.device);
	}

	static void createDefaultImageView(VkDevice device, ITexture& texture, const TextureDesc& desc)
	{
		TextureVk& tex = static_cast<TextureVk&>(texture);
		VkImageViewCreateInfo viewCreateInfo{};
		viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewCreateInfo.viewType = getVkImageViewType(desc.dimension);
		viewCreateInfo.format = tex.format;
		viewCreateInfo.image = tex.image;
		viewCreateInfo.subresourceRange.aspectMask = getVkAspectMask(tex.format);
		viewCreateInfo.subresourceRange.baseArrayLayer = 0;
		viewCreateInfo.subresourceRange.layerCount = desc.arraySize;
		viewCreateInfo.subresourceRange.baseMipLevel = 0;
		viewCreateInfo.subresourceRange.levelCount = desc.mipLevels;
		VkResult res = vkCreateImageView(device, &viewCreateInfo, nullptr, &tex.view);
		CHECK_VK_RESULT(res, "Could not create vkImageView");
	}

	ITexture* RenderDeviceVk::createTexture(const TextureDesc& desc)
	{
		TextureVk* tex = new TextureVk(m_Context, m_Allocator);
		tex->format = textureFormatToVkFormat(desc.format);

		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = getVkImageType(desc.dimension);
		imageCreateInfo.extent = { desc.width, desc.height, desc.depth };
		imageCreateInfo.mipLevels = desc.mipLevels;
		imageCreateInfo.arrayLayers = desc.depth;
		imageCreateInfo.format = tex->format;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = getVkImageUsageFlags(desc);
		imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.samples = getVkImageSampleCount(desc);
		imageCreateInfo.flags = getVkImageCreateFlags(desc.dimension);

		// Let the library select the optimal memory type, which will likely have VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT.
		VmaAllocationCreateInfo allocCreateInfo = {};
		allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
		allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
		allocCreateInfo.priority = 1.0f;
		VkResult err = vmaCreateImage(m_Allocator, &imageCreateInfo, &allocCreateInfo, &tex->image, &tex->allocation, nullptr);
		CHECK_VK_RESULT(err, "Could not to create vkImage");
		if (err != VK_SUCCESS)
		{
			delete tex;
			return nullptr;
		}
		tex->managed = true;
		createDefaultImageView(m_Context.device, *tex, desc);

		return tex;
	}

	IBuffer* RenderDeviceVk::createBuffer(const BufferDesc& desc)
	{
		BufferVk* buffer = new BufferVk(m_Context, m_Allocator);

		VkBufferCreateInfo bufferCI{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		bufferCI.size = desc.size;
		bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		switch (desc.usage)
		{
		case BufferUsage::VertexBuffer:
			bufferCI.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			break;
		case BufferUsage::IndexBuffer:
			bufferCI.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			break;
		case BufferUsage::IndirectBuffer:
			bufferCI.usage |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
			break;
		case BufferUsage::UniformBuffer:
			bufferCI.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			break;
		case BufferUsage::StorageBuffer:
			bufferCI.usage |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
			break;
		case BufferUsage::UniformTexelBuffer:
			bufferCI.usage |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
			break;
		case BufferUsage::StorageTexelBuffer:
			bufferCI.usage |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
			break;
		}

		VmaAllocationCreateInfo allocCI {};
		allocCI.usage = VMA_MEMORY_USAGE_AUTO;
		allocCI.priority = 1.0f;
		switch (desc.access)
		{
		case BufferAccess::GpuOnly:
			allocCI.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
			break;
		case BufferAccess::CpuWrite:
			allocCI.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
			break;
		case BufferAccess::CpuRead:
			allocCI.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
			break;
		}

		VkResult err = vmaCreateBuffer(m_Allocator, &bufferCI, &allocCI, &buffer->buffer, &buffer->allocation, nullptr);
		CHECK_VK_RESULT(err, "Could not create buffer");

		if (err != VK_SUCCESS)
		{
			delete buffer;
			buffer = nullptr;
		}

		return buffer;
	}

	IBuffer* RenderDeviceVk::createBuffer(const BufferDesc& desc, const void* data, size_t dataSize)
	{
		BufferVk* buffer = checked_cast<BufferVk*>(createBuffer(desc));

	}

	TextureVk* RenderDeviceVk::createTextureWithExistImage(const TextureDesc& desc, VkImage image)
	{
		auto tex = new TextureVk{ m_Context, m_Allocator };
		tex->image = image;
		tex->managed = false;
		tex->format = textureFormatToVkFormat(desc.format);
		tex->desc = desc;

		createDefaultImageView(m_Context.device, *tex, desc);

		return tex;
	}

	void RenderDeviceVk::setSwapChainImageAvailableSeamaphore(const VkSemaphore& semaophore)
	{
		m_SwapChainImgAavailableSemaphore = semaophore;
	}

	CommandBuffer* RenderDeviceVk::getOrCreateCommandBuffer()
	{
#if defined RHI_ENABLE_THREAD_RECORDING
		std::lock_guard(m_Mutex);
#endif
		CommandBuffer* cmdBuf;
		if (m_CommandBufferPool.empty())
		{
			cmdBuf = new CommandBuffer(m_Context);
			VkCommandPoolCreateInfo commandPoolCI{};
			commandPoolCI.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			commandPoolCI.queueFamilyIndex = m_QueueFamilyIndex;
			commandPoolCI.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

			VkResult err = vkCreateCommandPool(m_Context.device, &commandPoolCI, nullptr, &cmdBuf->vkCmdPool);
			CHECK_VK_RESULT(err, "Could not create vkCommandPool");

			VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
			commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			commandBufferAllocateInfo.commandPool = cmdBuf->vkCmdPool;
			commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			commandBufferAllocateInfo.commandBufferCount = 1;

			err = vkAllocateCommandBuffers(m_Context.device, &commandBufferAllocateInfo, &cmdBuf->vkCmdBuf);
			CHECK_VK_RESULT(err, "Could not create vkCommandBuffer");

			if (err != VK_SUCCESS)
			{
				delete cmdBuf;
				return nullptr;
			}
		}
		else
		{
			cmdBuf = m_CommandBufferPool.back();
			m_CommandBufferPool.pop_back();
		}

		return cmdBuf;
	}
}