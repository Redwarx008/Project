#pragma once

#include "rhi/Resource.h"

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <unordered_map>
#include <vector>

namespace rhi
{
	class MemoryResource
	{
	public:
		bool managed = true;
		VmaAllocation allocation = nullptr;
	};

	inline VkAccessFlags2 resourceStatesToVkAccessFlags2(ResourceState states);

	inline VkPipelineStageFlags2 resourceStatesToVkPipelineStageFlags2(ResourceState states);

}