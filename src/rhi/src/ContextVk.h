#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include <iostream>

#include "rhi/Rhi.h"

namespace rhi
{
    struct ContextVk
    {
        VkInstance instace;
        VkPhysicalDevice physicalDevice;
        VkDevice device;
    };
}