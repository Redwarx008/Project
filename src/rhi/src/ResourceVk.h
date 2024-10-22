#pragma once

#include "rhi/Resource.h"

#include <vk_mem_alloc.h>

#include <unordered_map>
#include <vector>

namespace rhi
{
	class TextureVk;
	class BufferVk;

	struct TextureBarrier
	{
		TextureVk* texture = nullptr;
		ResourceState stateBefore = ResourceState::Undefined;
		ResourceState stateAfter = ResourceState::Undefined;
	};

	struct BufferBarrier
	{
		BufferVk* buffer = nullptr;
		ResourceState stateBefore = ResourceState::Undefined;
		ResourceState stateAfter = ResourceState::Undefined;
	};

	class MemoryResource
	{
	public:
		bool managed = true;
		VmaAllocation allocation = nullptr;
	};

}