#include "SwapChainVk.h"
#include "RenderDeviceVk.h"
#include "ErrorVk.h"

#include <algorithm>

namespace rhi
{
	SwapChainVk* SwapChainVk::create(const SwapChainCreateInfo& swapChainCI)
	{
		return nullptr;
	}

	void SwapChainVk::createSurface(void* platformHandle, void* platformWindow)
	{
		VkResult err = VK_SUCCESS;

		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = (HINSTANCE)platformHandle;
		surfaceCreateInfo.hwnd = (HWND)platformWindow;
		err = vkCreateWin32SurfaceKHR(m_RenderDevice.getVkContext().instace, &surfaceCreateInfo, nullptr, &m_WindowSurface);

		CHECK_VK_RESULT(err, "Could not create surface!");

		VkBool32 isGraphicsSupportPresent;
		vkGetPhysicalDeviceSurfaceSupportKHR(m_RenderDevice.getVkContext().physicalDevice, m_RenderDevice.getQueueFamilyIndices().graphics.value(), m_WindowSurface, &isGraphicsSupportPresent);
		if (!isGraphicsSupportPresent)
		{
			LOG_ERROR("Could not support present!");
			return;
		}
	}

	void SwapChainVk::createVkSwapChain()
	{
		// Get list of supported surface formats
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_RenderDevice.getVkContext().physicalDevice, m_WindowSurface, &formatCount, NULL);
		assert(formatCount > 0);

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_RenderDevice.getVkContext().physicalDevice, m_WindowSurface, &formatCount, surfaceFormats.data());

		VkSurfaceFormatKHR selectedFormat = surfaceFormats[0];
		std::vector<VkFormat> preferredImageFormats = {
			rhi::textureFormatToVkFormat(m_ColorFormat),
			VK_FORMAT_B8G8R8A8_UNORM,
			VK_FORMAT_R8G8B8A8_UNORM,
		};

		for (auto& availableFormat : surfaceFormats) {
			if (std::find(preferredImageFormats.begin(), preferredImageFormats.end(), availableFormat.format) != preferredImageFormats.end()) {
				selectedFormat = availableFormat;
				break;
			}
		}

		if (selectedFormat.format != rhi::textureFormatToVkFormat(m_ColorFormat))
		{
			std::stringstream ss;
			ss << "Requested color format is not supported and will be replaced by " << getFormatInfo(vkFormatToTextureFormat(selectedFormat.format)).name;
			logMsg(MessageSeverity::Warning, __FUNCTION__, __LINE__, ss);
		}

		// Store the current swap chain handle so we can use it later on to ease up recreation
		VkSwapchainKHR oldSwapchain = m_SwapChain;

		// Get physical device surface properties and formats
		VkSurfaceCapabilitiesKHR surfCaps;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_RenderDevice.getVkContext().physicalDevice, m_WindowSurface, &surfCaps);

		// Get available present modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_RenderDevice.getVkContext().physicalDevice, m_WindowSurface, &presentModeCount, NULL);
		assert(presentModeCount > 0);

		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_RenderDevice.getVkContext().physicalDevice, m_WindowSurface, &presentModeCount, presentModes.data());

		VkExtent2D swapchainExtent = {};
		// If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
		if (surfCaps.currentExtent.width == (uint32_t)-1)
		{
			// If the surface size is undefined, the size is set to
			// the size of the images requested.
			swapchainExtent.width = std::clamp(m_Width, surfCaps.minImageExtent.width, surfCaps.maxImageExtent.width);
			swapchainExtent.height = std::clamp(m_Height, surfCaps.minImageExtent.height, surfCaps.maxImageExtent.height);
		}
		else
		{
			// If the surface size is defined, the swap chain size must match
			swapchainExtent = surfCaps.currentExtent;
		}

		// Select a present mode for the swapchain

		// The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
		// This mode waits for the vertical blank ("v-sync")
		VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

		// If v-sync is not requested, try to find a mailbox mode
		// It's the lowest latency non-tearing present mode available
		if (!m_VSyncEnabled)
		{
			for (size_t i = 0; i < presentModeCount; i++)
			{
				if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}
				if (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)
				{
					swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				}
			}
		}

		// Determine the number of images
		uint32_t desiredNumberOfSwapchainImages = surfCaps.minImageCount + 1;
		if ((surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfCaps.maxImageCount))
		{
			desiredNumberOfSwapchainImages = surfCaps.maxImageCount;
		}

		// Find the transformation of the surface
		VkSurfaceTransformFlagsKHR preTransform;
		if (surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			// We prefer a non-rotated transform
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			preTransform = surfCaps.currentTransform;
		}

		// Find a supported composite alpha format (not all devices support alpha opaque)
		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		// Simply select the first composite alpha format available
		std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		};
		for (auto& compositeAlphaFlag : compositeAlphaFlags) {
			if (surfCaps.supportedCompositeAlpha & compositeAlphaFlag) {
				compositeAlpha = compositeAlphaFlag;
				break;
			};
		}

		VkSwapchainCreateInfoKHR swapchainCI = {};
		swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCI.surface = m_WindowSurface;
		swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
		swapchainCI.imageFormat = selectedFormat.format;
		swapchainCI.imageColorSpace = selectedFormat.colorSpace;
		swapchainCI.imageExtent = { swapchainExtent.width, swapchainExtent.height };
		swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
		swapchainCI.imageArrayLayers = 1;
		swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCI.queueFamilyIndexCount = 0;
		swapchainCI.presentMode = swapchainPresentMode;
		// Setting oldSwapChain to the saved handle of the previous swapchain aids in resource reuse and makes sure that we can still present already acquired images
		swapchainCI.oldSwapchain = oldSwapchain;
		// Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
		swapchainCI.clipped = VK_TRUE;
		swapchainCI.compositeAlpha = compositeAlpha;

		// Enable transfer source on swap chain images if supported
		if (surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		// Enable transfer destination on swap chain images if supported
		if (surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		VkResult err = vkCreateSwapchainKHR(m_RenderDevice.getVkContext().device, &swapchainCI, nullptr, &m_SwapChain);

		CHECK_VK_RESULT(err, "Could not create swapchain!");

		// If an existing swap chain is re-created, destroy the old swap chain
		// This also cleans up all the presentable images
		if (oldSwapchain != VK_NULL_HANDLE)
		{
			m_ColorTextures.clear();
			vkDestroySwapchainKHR(m_RenderDevice.getVkContext().device, oldSwapchain, nullptr);
			oldSwapchain = VK_NULL_HANDLE;
		}

		uint32_t imageCount;
		vkGetSwapchainImagesKHR(m_RenderDevice.getVkContext().device, m_SwapChain, &imageCount, NULL);
		std::vector<VkImage> images;
		err = vkGetSwapchainImagesKHR(m_RenderDevice.getVkContext().device, m_SwapChain, &imageCount, images.data());
		CHECK_VK_RESULT(err, "Failed to get swap chain images");

		m_ColorTextures.resize(imageCount);

		TextureDesc colorTextureDesc;
		colorTextureDesc.width = swapchainExtent.width;
		colorTextureDesc.height = swapchainExtent.height;
		colorTextureDesc.format = m_ColorFormat;
		colorTextureDesc.initialState = rhi::ResourceStates::Present;
		for (int i = 0; i < m_ColorTextures.size(); ++i)
		{
			m_ColorTextures[i] = std::unique_ptr<ITexture>(m_RenderDevice.createTextureWithExistImage(colorTextureDesc, images[i]));
		}

	}
}