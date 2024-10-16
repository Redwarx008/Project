#pragma once

#include "rhi/Resource.h"

#include <vk_mem_alloc.h>

namespace rhi
{
	class MemoryResource
	{
	public:
		bool managed = true;
		VmaAllocation allocation = nullptr;
	};
}