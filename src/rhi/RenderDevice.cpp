#include "RenderDevice.h"

using namespace rhi;

RenderDevice::RenderDevice(const RenderDeviceCreateInfo& createInfo)
{
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

	if (createInfo.requiredExtensions.size() > 0)
	{
		for (const char* requiredExtension : createInfo.requiredExtensions)
		{
			// Output message if requested extension is not available
			if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), requiredExtension) == supportedInstanceExtensions.end())
			{
				std::cerr << "Enabled instance extension \"" << requiredExtension << "\" is not present at instance level\n";
			}
			instanceExtensions.push_back(enabledExtension);
		}
	}
}


std::unique_ptr<RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo)
{
	auto renderDevice = std::make_unique<RenderDevice>(createInfo);
	return renderDevice;
}